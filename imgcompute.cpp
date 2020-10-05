#include "imgcompute.h"
#include <time.h>
#include <omp.h>

using namespace std;

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"

#include <Windows.h>

//��������GPU�˷���ʽ
enum Type
{
	Mode1 = 1,   //Mode 1 :��ÿһ��C[i][j]���ֱ����һ���߳�
	Mode2 = 2     //Mode 2 :����һ���߳���������һ��C[i][j]��ͨ��C(i,j) = sum { A(i,k)*B(k,j) }���֣����ǻ�������ϸ�Ȼ��֣�
				 //           sub(i,j) = sum{A(i,ksub+offsetA)*B(ksub+offsetB,j)}  0<=ksub < blockSize
				 //            C(i, j) = sum{ Csub(i, j) }
				 //            ���ǰѾ���ֳ�n*n������ӿ飬Ȼ��ÿһ��block��������ӿ�i �� �ӿ�j���ӳ˻���������Ϻ��������ɡ�������Ҫʹ���˹����Դ����Ż���
};

extern "C" cudaError_t addWithCuda(float* c, const float* a, const float* b, unsigned int WA, unsigned int HA, unsigned int WB, unsigned int HB, Type mode);

extern "C" double* main3(int* datatemp, int dim);

clock_t start, stop; //clock_tΪclock()�������صı�������
double duration;

void imagecompute_gpu(Imagdata* imagdata, CovData* imagecov) {

	//��ʱ
	//unsigned int tick_cov = GetTickCount();

	//��ʱ��ʼ
	clock_t t_start, t_stop;
	double duration;
	t_start = clock(); 

	long count, count1;
	long long count2;

	//��ȡ����߳���
	int numProcs = omp_get_num_procs();
	 
	imagecov->data = new covBYTE[sizeof(covBYTE) * imagdata->Dim * imagdata->Dim];
	imagecov->row = imagdata->Dim;
	imagecov->column = imagdata->Dim;

	cBYTE* datatemp = new cBYTE[sizeof(cBYTE) * imagdata->rawSize]; // rawSize ��512x680x224,8000W

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
	//printf("(imgcompute.cpp)���ؾ�ֵ��������...\n");

	//// ��ʱ����
	//t_stop = clock();
	//duration = (double)(t_stop - t_start) / CLK_TCK;

	//printf("666ʱ�䣺%fs\n", duration);
	

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
	// ����GPU������ ��ֵ�� imagecov.data


	delete[]datatemp;

	//printf("(imgcompute.cpp)GPUЭ����������\n");
	//printf("Э������㲿�֣�GPU�Ż�������ʱ��Ϊ : %dms\n", GetTickCount() - tick_cov);


	//��ʱ����
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def imagecompute_gpu������Э����. ʱ�䣺%fs\n", duration);

	//-----------------------------------------------------------------------------------------------------------


	//cout << "(imgcompute.cpp)Э����������\n" <<endl;

}
