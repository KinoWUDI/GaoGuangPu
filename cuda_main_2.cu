#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//CUDA RunTime API
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "device_functions.h"

#define THREAD_NUM 256

#define raw_row 512*680
#define raw_column 224

#define MATRIX_SIZE raw_row*raw_column

const int blocks_num = 16;


//// __global__ 函数 并行计算矩阵乘法
//__global__ static void matMultCUDA(Imagdata* imagdata, CovData* imagecov, cBYTE* datatemp, clock_t* time)
//{
//
//	//表示目前的 thread 是第几个 thread（由 0 开始计算）
//	const int tid = threadIdx.x;
//
//	//表示目前的 thread 属于第几个 block（由 0 开始计算）
//	const int bid = blockIdx.x;
//
//	//// 只适用方阵
//	////从 bid 和 tid 计算出这个 thread 应该计算的 row 和 column
//	//const int idx = bid * THREAD_NUM + tid;
//	//const int row = idx / n;
//	//const int column = idx % n;
//
//	//记录运算开始的时间
//
//	//只在 thread 0（即 threadIdx.x = 0 的时候）进行记录，每个 block 都会记录开始时间及结束时间
//	if (tid == 0) time[bid] = clock();
//	if (tid == 0)
//		printf("Hello thread %d, f=%f\n", tid);
//
//	/*
//	计算协方差过程开始
//	*/
//
//	long count;
//	long count2;
//
//	imagecov->row = imagdata->Dim;
//	imagecov->column = imagdata->Dim;
//
//	for (int i = 0; i < imagdata->Dim; ++i) {
//		count = 0;
//		int j = 0;
//		for (j = 0; j < imagdata->imgSize; ++j) {
//			count += imagdata->data[i * imagdata->imgSize + j];
//		}
//		int mean = count / imagdata->imgSize;
//		for (j = 0; j < imagdata->imgSize; ++j) {
//			datatemp[i * imagdata->imgSize + j] = imagdata->data[i * imagdata->imgSize + j] - mean;
//		}
//	}
//	//cout << "(imgcompute.cpp)像素均值差计算完成\n" << endl;
//	printf("(imgcompute.cpp)像素均值差计算完成...\n");
//	//-----------------------------------------------------------------------------------------------------------
//	//此部分运行时间为45秒。并发处理后为21秒。将循环次减半，时间缩短为12s
//
//	for (int i = 0; i < imagdata->Dim; ++i)
//	{
//		int j;
//		for (j = i; j < imagdata->Dim; ++j)
//		{
//			count2 = 0;
//			int k;
//			//for (k = 0; k < imagdata->imgSize; ++k) {
//
//			//	count2 += datatemp[i * imagdata->imgSize + k] * datatemp[j * imagdata->imgSize + k];
//			//}
//			for (k = 0; k < imagdata->imgSize; ++k) {
//
//				count2 += datatemp[i * imagdata->imgSize + k] * datatemp[j * imagdata->imgSize + k];
//			}
//			imagecov->data[i * (imagecov->row) + j] = count2 / (imagdata->imgSize - 1);
//			if (i != j) {
//				imagecov->data[j * (imagecov->row) + i] = count2 / (imagdata->imgSize - 1);
//			}
//		}
//	}
//
//	delete[]datatemp;
//	printf("(imgcompute.cpp)协方差计算完成\n");
//
//	/*
//	计算协方差过程结束
//	*/
//
//
//	//计算时间,记录结果，只在 thread 0（即 threadIdx.x = 0 的时候）进行，每个 block 都会记录开始时间及结束时间
//	if (tid == 0)
//	{
//		time[bid + blocks_num] = clock();
//	}
//
//}
//
//
//__global__ void cudatest() {
//	printf("success!");
//}
//
//
//extern "C" void cuda_juzhen(RawImag * myraw, Imagdata * imagdata, CovData * imagecov)
//{
//	clock_t* time;
//
//	printf("1\n");
//
//	//CPU和GPU分配内存 同一个结构体要分开声明（实例化）
//	//CPU分配内存
//	imagecov->data = new covBYTE[sizeof(covBYTE) * imagdata->Dim * imagdata->Dim];
//	cBYTE* datatemp = new cBYTE[sizeof(cBYTE) * imagdata->rawSize]; // rawSize 是512x680x224,8000W
//
//	printf("2\n");
//
//
//	//cudaMalloc 取得一块显卡内存 
//	//结构体分配内存要分配结构体本身，以及结构体中每一个变量
//
//
//	Imagdata* cuda_imagdata;
//	CovData* cuda_imagecov;
//	cBYTE* cuda_datatemp;
//
//	cuda_imagdata = new Imagdata[sizeof(Imagdata)]; // 申请结构体数组（指针）的内存
//	cuda_imagecov = new CovData[sizeof(CovData)];
//	cuda_datatemp = new cBYTE[sizeof(cBYTE)];
//
//
//	//memset(&cuda_imagdata, 0, sizeof(Imagdata));
//	//memset(&cuda_imagecov, 0, sizeof(CovData));
//	//memset(&cuda_datatemp, 0, sizeof(cBYTE));
//
//
//	cuda_imagdata->Col = imagdata->Col;
//	cuda_imagdata->Row = imagdata->Row;
//	cuda_imagdata->Dim = imagdata->Dim;
//	cuda_imagdata->rawSize = imagdata->rawSize;
//	cuda_imagdata->imgSize = imagdata->imgSize;
//
//	cuda_imagecov->row = imagdata->Dim;
//	cuda_imagecov->column = imagdata->Dim;
//
//
//	cuda_imagdata->data = new covBYTE[sizeof(covBYTE) * imagdata->Dim * imagdata->Dim];;
//	cuda_datatemp = new cBYTE[sizeof(cBYTE) * imagdata->rawSize]; // rawSize 是512x680x224,8000W;
//
//	//cudaMalloc((void**)&cuda_imagdata->Row, sizeof(int));
//	//cudaMalloc((void**)&cuda_imagdata->Col, sizeof(int));
//	//cudaMalloc((void**)&cuda_imagdata->Dim, sizeof(int));
//	//cudaMalloc((void**)&cuda_imagdata->rawSize, sizeof(int));
//	//cudaMalloc((void**)&cuda_imagdata->imgSize, sizeof(int));
//
//	//cudaMalloc((void**)&cuda_imagdata, sizeof(Imagdata));
//	cudaMalloc((void**)&cuda_imagdata->data, sizeof(cBYTE) * imagdata->rawSize);
//
//	printf("2.1\n");
//
//	//cudaMalloc((void**)&cuda_imagecov->row, sizeof(int));
//	//cudaMalloc((void**)&cuda_imagecov->column, sizeof(int));
//
//
//	//cudaMalloc((void**)&cuda_imagecov, sizeof(CovData));
//
//	cuda_imagecov->data = new cBYTE[sizeof(cBYTE) * imagdata->rawSize];
//	cudaMalloc((void**)&cuda_imagecov->data, sizeof(covBYTE) * imagdata->Dim * imagdata->Dim);
//
//	printf("2.3\n");
//
//	cudaMalloc((void**)&cuda_datatemp, sizeof(cBYTE) * imagdata->rawSize);
//
//	cudaMalloc((void**)&time, sizeof(clock_t) * blocks_num * 2);
//
//	printf("3\n");
//
//
//	//cudaMemcpy 将产生的矩阵复制到显卡内存中
//	//cudaMemcpyHostToDevice - 从内存复制到显卡内存
//	//cudaMemcpyDeviceToHost - 从显卡内存复制到内存
//
//	//cudaMemcpy(cuda_imagdata, imagdata, sizeof(cBYTE) * imagdata->rawSize, cudaMemcpyHostToDevice);
//	cudaMemcpy(cuda_imagdata->data, imagdata->data, sizeof(cBYTE) * imagdata->rawSize, cudaMemcpyHostToDevice);
//
//	//cudaMemcpy(cuda_imagecov, imagecov, sizeof(covBYTE) * imagdata->rawSize, cudaMemcpyHostToDevice);
//	cudaMemcpy(cuda_imagecov->data, imagecov->data, sizeof(covBYTE) * imagdata->rawSize, cudaMemcpyHostToDevice);
//
//	cudaMemcpy(cuda_datatemp, datatemp, sizeof(cBYTE) * imagdata->rawSize, cudaMemcpyHostToDevice);
//
//	printf("4\n");
//
//	// 在CUDA 中执行函数 语法：函数名称<<<block 数目, thread 数目, shared memory 大小>>>(参数...);
//	//matMultCUDA <<< blocks_num, THREAD_NUM>>> (cuda_imag;data, cuda_imagecov, cuda_datatemp, time);
//	cudatest << < 1, 64 >> > ;
//
//	cudaError_t cudaStatus = cudaGetLastError();
//	if (cudaStatus != cudaSuccess)
//	{
//		fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
//	}
//
//
//	printf("5\n");
//
//	/*把结果从显示芯片复制回主内存*/
//
//	clock_t time_use[blocks_num * 2];
//
//	//cudaMemcpy 将结果从显存中复制回内存
//	Imagdata* new_imagdata;
//	CovData* new_imagecov;
//
//	new_imagdata = new Imagdata[sizeof(Imagdata)]; // 申请结构体数组（指针）的内存
//	new_imagecov = new CovData[sizeof(CovData)];
//
//	//cudaMemcpy(new_imagdata, cuda_imagdata, sizeof(cBYTE) * imagdata->rawSize, cudaMemcpyDeviceToHost);
//	cudaMemcpy(new_imagdata->data, cuda_imagdata->data, sizeof(cBYTE) * imagdata->rawSize, cudaMemcpyDeviceToHost);
//	//cudaMemcpy(new_imagecov, cuda_imagecov, sizeof(covBYTE) * imagdata->rawSize, cudaMemcpyDeviceToHost);
//	cudaMemcpy(new_imagecov->data, cuda_imagecov->data, sizeof(covBYTE) * imagdata->rawSize, cudaMemcpyDeviceToHost);
//
//	cudaMemcpy(&time_use, time, sizeof(clock_t) * blocks_num * 2, cudaMemcpyDeviceToHost);
//
//	printf("6\n");
//
//	//Free
//	cudaFree(cuda_imagdata->data);
//	//cudaFree(cuda_imagdata);
//
//	cudaFree(cuda_imagecov->data);
//	//cudaFree(cuda_imagdata);
//
//	cudaFree(cuda_datatemp);
//	cudaFree(time);
//
//	//把每个 block 最早的开始时间，和最晚的结束时间相减，取得总运行时间
//	clock_t min_start, max_end;
//
//	min_start = time_use[0];
//
//	max_end = time_use[blocks_num];
//
//	for (int i = 1; i < blocks_num; i++)
//	{
//		if (min_start > time_use[i]) min_start = time_use[i];
//
//		if (max_end < time_use[i + blocks_num]) max_end = time_use[i + blocks_num];
//	}
//
//	//核函数运行时间
//	clock_t final_time = max_end - min_start;
//
//	printf("gputime: %d\n", final_time);
//
//}
