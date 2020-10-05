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


//// __global__ ���� ���м������˷�
//__global__ static void matMultCUDA(Imagdata* imagdata, CovData* imagecov, cBYTE* datatemp, clock_t* time)
//{
//
//	//��ʾĿǰ�� thread �ǵڼ��� thread���� 0 ��ʼ���㣩
//	const int tid = threadIdx.x;
//
//	//��ʾĿǰ�� thread ���ڵڼ��� block���� 0 ��ʼ���㣩
//	const int bid = blockIdx.x;
//
//	//// ֻ���÷���
//	////�� bid �� tid �������� thread Ӧ�ü���� row �� column
//	//const int idx = bid * THREAD_NUM + tid;
//	//const int row = idx / n;
//	//const int column = idx % n;
//
//	//��¼���㿪ʼ��ʱ��
//
//	//ֻ�� thread 0���� threadIdx.x = 0 ��ʱ�򣩽��м�¼��ÿ�� block �����¼��ʼʱ�估����ʱ��
//	if (tid == 0) time[bid] = clock();
//	if (tid == 0)
//		printf("Hello thread %d, f=%f\n", tid);
//
//	/*
//	����Э������̿�ʼ
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
//	//cout << "(imgcompute.cpp)���ؾ�ֵ��������\n" << endl;
//	printf("(imgcompute.cpp)���ؾ�ֵ��������...\n");
//	//-----------------------------------------------------------------------------------------------------------
//	//�˲�������ʱ��Ϊ45�롣���������Ϊ21�롣��ѭ���μ��룬ʱ������Ϊ12s
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
//	printf("(imgcompute.cpp)Э����������\n");
//
//	/*
//	����Э������̽���
//	*/
//
//
//	//����ʱ��,��¼�����ֻ�� thread 0���� threadIdx.x = 0 ��ʱ�򣩽��У�ÿ�� block �����¼��ʼʱ�估����ʱ��
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
//	//CPU��GPU�����ڴ� ͬһ���ṹ��Ҫ�ֿ�������ʵ������
//	//CPU�����ڴ�
//	imagecov->data = new covBYTE[sizeof(covBYTE) * imagdata->Dim * imagdata->Dim];
//	cBYTE* datatemp = new cBYTE[sizeof(cBYTE) * imagdata->rawSize]; // rawSize ��512x680x224,8000W
//
//	printf("2\n");
//
//
//	//cudaMalloc ȡ��һ���Կ��ڴ� 
//	//�ṹ������ڴ�Ҫ����ṹ�屾���Լ��ṹ����ÿһ������
//
//
//	Imagdata* cuda_imagdata;
//	CovData* cuda_imagecov;
//	cBYTE* cuda_datatemp;
//
//	cuda_imagdata = new Imagdata[sizeof(Imagdata)]; // ����ṹ�����飨ָ�룩���ڴ�
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
//	cuda_datatemp = new cBYTE[sizeof(cBYTE) * imagdata->rawSize]; // rawSize ��512x680x224,8000W;
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
//	//cudaMemcpy �������ľ����Ƶ��Կ��ڴ���
//	//cudaMemcpyHostToDevice - ���ڴ渴�Ƶ��Կ��ڴ�
//	//cudaMemcpyDeviceToHost - ���Կ��ڴ渴�Ƶ��ڴ�
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
//	// ��CUDA ��ִ�к��� �﷨����������<<<block ��Ŀ, thread ��Ŀ, shared memory ��С>>>(����...);
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
//	/*�ѽ������ʾоƬ���ƻ����ڴ�*/
//
//	clock_t time_use[blocks_num * 2];
//
//	//cudaMemcpy ��������Դ��и��ƻ��ڴ�
//	Imagdata* new_imagdata;
//	CovData* new_imagecov;
//
//	new_imagdata = new Imagdata[sizeof(Imagdata)]; // ����ṹ�����飨ָ�룩���ڴ�
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
//	//��ÿ�� block ����Ŀ�ʼʱ�䣬������Ľ���ʱ�������ȡ��������ʱ��
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
//	//�˺�������ʱ��
//	clock_t final_time = max_end - min_start;
//
//	printf("gputime: %d\n", final_time);
//
//}
