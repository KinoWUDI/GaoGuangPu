
/*
	矩阵乘法，CPU版本和GPU版本的对比
*/

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <string>
#include <malloc.h>
#include <omp.h>

// typedef int gpuByte;

//用于指示不同的GPU 优化版本
enum Type
{
	Mode1 = 1,   //Mode 1 :将每一个C[i][j]都分别分配一个线程
	Mode2 = 2     //Mode 2 :不让一个线程完整计算一个C[i][j]，通过C(i,j) = sum { A(i,k)*B(k,j) }发现，我们还可以再细度划分：
				 //           sub(i,j) = sum{A(i,ksub+offsetA)*B(ksub+offsetB,j)}  0<=ksub < blockSize
				 //            C(i, j) = sum{ Csub(i, j) }
				 //            就是把矩阵分成n*n个大的子块，然后每一个block负责计算子块i 和 子块j的子乘积，计算完毕后加起来则可。这里主要使用了共享显存作优化。
};

extern "C" cudaError_t addWithCuda(double* c, const double* a, const double* b, unsigned int WA, unsigned int HA, unsigned int WB, unsigned int HB, Type mode);


/* gpuMatMultKernel：GPU下矩阵乘法核函数
*  a:第一个矩阵指针，表示a[M][N]
*  b:第二个矩阵指针，表示b[N][S]
*  c:结果矩阵，表示c[M][S]
*/
__global__ void MatrixMulGPU_1(double* c, const double* a, const double* b, unsigned int WA, unsigned int WB)
{
	int sum = 0;
	//找出该线程所在的行和列
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	//线程Thread(row, col)负责计算C(row, col)
	for (int i = 0; i < WB; ++i)
	{
		sum += a[row * WA + i] * b[i * WB + col];
	}

	c[row * WB + col] = sum / (512 * 680 - 1);
}

/*
template<int BLOCK_SIZE> __global__ void MatrixMulGPU_2(int* c, const int* a, const int* b, unsigned int WA, unsigned int WB)
{
	// Block index
	int bx = blockIdx.x;
	int by = blockIdx.y;

	// Thread index
	int tx = threadIdx.x;
	int ty = threadIdx.y;

	// Index of the first sub-matrix of A processed by the block
	int aBegin = WA * BLOCK_SIZE * by;

	// Index of the last sub-matrix of A processed by the block
	int aEnd = aBegin + WA - 1;

	// Step size used to iterate through the sub-matrices of A
	int aStep = BLOCK_SIZE;

	// Index of the first sub-matrix of B processed by the block
	int bBegin = BLOCK_SIZE * bx;

	// Step size used to iterate through the sub-matrices of B
	int bStep = BLOCK_SIZE * WB;

	// Csub is used to store the element of the block sub-matrix
	// that is computed by the thread
	int Csub = 0;

	// Loop over all the sub-matrices of A and B
	// required to compute the block sub-matrix
	for (int i = aBegin, j = bBegin;
		i <= aEnd;
		i += aStep, j += bStep)
	{

		// Declaration of the shared memory array As used to
		// store the sub-matrix of A
		__shared__ int As[BLOCK_SIZE][BLOCK_SIZE];

		// Declaration of the shared memory array Bs used to
		// store the sub-matrix of B
		__shared__ int Bs[BLOCK_SIZE][BLOCK_SIZE];

		// Load the matrices from device memory
		// to shared memory; each thread loads
		// one element of each matrix
		As[ty][tx] = a[i + WA * ty + tx];
		Bs[ty][tx] = b[j + WB * ty + tx];

		// Synchronize to make sure the matrices are loaded
		__syncthreads();

		// Multiply the two matrices together;
		// each thread computes one element
		// of the block sub-matrix
#pragma unroll

		for (int k = 0; k < BLOCK_SIZE; ++k)
		{
			Csub += As[ty][k] * Bs[k][tx];
		}

		// Synchronize to make sure that the preceding
		// computation is done before loading two new
		// sub-matrices of A and B in the next iteration
		__syncthreads();
	}

	// Write the block sub-matrix to device memory;
	// each thread writes one element
	int k = WB * BLOCK_SIZE * by + BLOCK_SIZE * bx;
	c[k + WB * ty + tx] = Csub;
}
*/

//print the matrix
void printMatrix(int* m_Matrix, int W, int H)
{
	for (int i = 0; i < W * H; ++i)
	{
		printf("%d ", m_Matrix[i]);
		if (i % W == 0 && i != 0) printf("\n");
	}
	printf("\n");
}


extern "C" double* main3(int* datatemp,int dim)
{
	const int width_A = 512 * 680;
	const int height_A = dim;
	const int width_B = dim;
	const int height_B = 512 * 680;

	double* B = (double*)malloc(sizeof(double) * height_B * width_B);
	double* A = (double*)malloc(sizeof(double) * height_A * width_A);
	//int* C = (int*)malloc(sizeof(int) * height_A * width_B);
	double* D = (double*)malloc(sizeof(double) * height_A * width_B);
	//int* E = (int*)malloc(sizeof(int) * height_A * width_B);

	//memset 为初始化 置0
	//memset(A, 0.0, sizeof(int) * height_A * width_A);
	//memset(B, 0.0, sizeof(int) * height_B * width_B);
	//memset(C, 0.0, sizeof(int) * height_A * width_B);
	memset(D, 0.0, sizeof(double) * height_A * width_B);
	//memset(E, 0.0, sizeof(int) * height_A * width_B);

	//获取最大线程数
	int numProcs = omp_get_num_procs();

	// 给 A/B矩阵 赋值
	int i;

	for (i = 0; i < height_A * width_A; ++i)
	{
		A[i] = datatemp[i];
		B[i] = datatemp[i];
	}

	//GPU mode1
	Type m_Mode = Mode1;

	unsigned int tick2 = GetTickCount();
	cudaError_t cudaStatus = addWithCuda(D, A, B, width_A, height_A, width_B, height_B, m_Mode);

	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "addWithCuda failed!\n");
		//return 1;
	}
	//printf("GPU mode1 use time : %dms\n", GetTickCount() - tick2);


	////GPU mode2
	//m_Mode = Mode2;
	//unsigned int tick3 = GetTickCount();
	//cudaStatus = addWithCuda(E, A, B, width_A, height_A, width_B, height_B, m_Mode);
	//if (cudaStatus != cudaSuccess)
	//{
	//    fprintf(stderr, "addWithCuda failed!\n");
	//    return 1;
	//}
	//printf("GPU mode2 use time : %dms\n", GetTickCount() - tick3);

	//输出结果矩阵 查看
	//printMatrix(C, 3, 3);

	//唯一需要的输出
	//printMatrix(D, 3, 3);

	//printMatrix(E, 3, 3);

	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess)
	{
		fprintf(stderr, "cudaDeviceReset failed!");
		//return 1;
	}

	return D;
}

// Helper function for using CUDA to add vectors in parallel.
extern "C" cudaError_t addWithCuda(double* c, const double* a, const double* b, unsigned int WA, unsigned int HA, unsigned int WB, unsigned int HB, Type mode)
{
	double* dev_a = 0;
	double* dev_b = 0;
	double* dev_c = 0;
	cudaError_t cudaStatus;


	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
	}

	// Allocate GPU buffers for three vectors (two input, one output)    .
	cudaStatus = cudaMalloc((void**)&dev_c, HA * WB * sizeof(double)); //HA 224 WB 224
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
	}

	cudaStatus = cudaMalloc((void**)&dev_a, HA * WA * sizeof(double)); //HA 224 WA 512*680
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
	}

	cudaStatus = cudaMalloc((void**)&dev_b, HB * WB * sizeof(double)); //HB 512*680 WB 224
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
	}

	// Copy input vectors from host memory to GPU buffers.
	cudaStatus = cudaMemcpy(dev_a, a, HA * WA * sizeof(double), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
	}

	cudaStatus = cudaMemcpy(dev_b, b, HB * WB * sizeof(double), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
	}

	//为每一个C[i][j]设置一个线程进行计算
	int block_size = 1;

	dim3 Threads(block_size, block_size);
	dim3 Blocks(WB / block_size, HA / block_size);

	// Launch a kernel on the GPU with one thread for each element.
	if (mode == Mode1)
	{

		MatrixMulGPU_1 << <Blocks, Threads >> > (dev_c, dev_a, dev_b, WA, WB);

	}

	//if (mode == Mode2)
	//{

	//	MatrixMulGPU_2<16> << <Blocks, Threads >> > (dev_c, dev_a, dev_b, WA, WB);

	//}

	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
	}

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
	}

	// Copy output vector from GPU buffer to host memory.
	cudaStatus = cudaMemcpy(c, dev_c, HA * WB * sizeof(double), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
	}

	return cudaStatus;
}

