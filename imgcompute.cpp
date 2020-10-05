#include "imgcompute.h"
#include <time.h>
#include <omp.h>

using namespace std;

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"

#include <Windows.h>

//声明两种GPU乘法方式
enum Type
{
	Mode1 = 1,   //Mode 1 :将每一个C[i][j]都分别分配一个线程
	Mode2 = 2     //Mode 2 :不让一个线程完整计算一个C[i][j]，通过C(i,j) = sum { A(i,k)*B(k,j) }发现，我们还可以再细度划分：
				 //           sub(i,j) = sum{A(i,ksub+offsetA)*B(ksub+offsetB,j)}  0<=ksub < blockSize
				 //            C(i, j) = sum{ Csub(i, j) }
				 //            就是把矩阵分成n*n个大的子块，然后每一个block负责计算子块i 和 子块j的子乘积，计算完毕后加起来则可。这里主要使用了共享显存作优化。
};

extern "C" cudaError_t addWithCuda(float* c, const float* a, const float* b, unsigned int WA, unsigned int HA, unsigned int WB, unsigned int HB, Type mode);

extern "C" double* main3(int* datatemp, int dim);

clock_t start, stop; //clock_t为clock()函数返回的变量类型
double duration;

void imagecompute_gpu(Imagdata* imagdata, CovData* imagecov) {

	//计时
	//unsigned int tick_cov = GetTickCount();

	//计时开始
	clock_t t_start, t_stop;
	double duration;
	t_start = clock(); 

	long count, count1;
	long long count2;

	//获取最大线程数
	int numProcs = omp_get_num_procs();
	 
	imagecov->data = new covBYTE[sizeof(covBYTE) * imagdata->Dim * imagdata->Dim];
	imagecov->row = imagdata->Dim;
	imagecov->column = imagdata->Dim;

	cBYTE* datatemp = new cBYTE[sizeof(cBYTE) * imagdata->rawSize]; // rawSize 是512x680x224,8000W

#pragma omp parallel for default(shared) private(count) num_threads(numProcs)
	for (int i = 0; i < imagdata->Dim; ++i) {
		count = 0;
		for (int j = 0; j < imagdata->imgSize; ++j) {
			count += imagdata->data[i * imagdata->imgSize + j];
		}
		// int mean = count / imagdata->imgSize;
		for (int j = 0; j < imagdata->imgSize; ++j) {
			datatemp[i * imagdata->imgSize + j] = imagdata->data[i * imagdata->imgSize + j] - count / imagdata->imgSize;
		}
	}
	//printf("(imgcompute.cpp)像素均值差计算完成...\n");

	//// 计时结束
	//t_stop = clock();
	//duration = (double)(t_stop - t_start) / CLK_TCK;

	//printf("666时间：%fs\n", duration);
	

#pragma omp critical
	{
		double* gpu_output;
		gpu_output = new double[sizeof(double) * imagdata->Dim * imagdata->Dim];

		gpu_output = main3(datatemp, imagdata->Dim);

		for (int i = 0; i < imagdata->Dim * imagdata->Dim; ++i) {
			imagecov->data[i] = gpu_output[i];
		}

		delete[]gpu_output;
	}
	// 接受GPU计算结果 赋值给 imagecov.data


	delete[]datatemp;

	//printf("(imgcompute.cpp)GPU协方差计算完成\n");
	//printf("协方差计算部分，GPU优化后运行时间为 : %dms\n", GetTickCount() - tick_cov);


	//计时结束
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def imagecompute_gpu：计算协方差. 时间：%fs\n", duration);

	//-----------------------------------------------------------------------------------------------------------


	//cout << "(imgcompute.cpp)协方差计算完成\n" <<endl;

}
