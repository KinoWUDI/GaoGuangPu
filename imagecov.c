#include"imagecov.h"
#include <omp.h>

#include<time.h>

void imagecov(RawImag* myraw, Imagdata* imagdata, Image* imagmean) {

	//clock_t start, stop; //clock_t为clock()函数返回的变量类型
	//double duration;

	// 计时开始
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();

	//获取最大线程数
	int numProcs = omp_get_num_procs();

	//计算均值
	long count, count1;
	long long count2;
	covBYTE equ1, equ2;

	imagmean->data = (rBYTE*)malloc(sizeof(rBYTE) * myraw->imgSize);
	int i = 0;
// #pragma omp parallel for private（iindex）shared（array）
#pragma omp parallel for default(shared) private(count) num_threads(numProcs)
	for (i = 0; i < myraw->imgSize; ++i) {
		count = 0;
		for (int j = 0; j < myraw->Dim; ++j) {
			count += myraw->data[j * myraw->imgSize + i];
		}
		imagmean->data[i] = (int)count / myraw->Dim;

	}
	imagmean->Col = myraw->Col;
	imagmean->Row = myraw->Row;
	//printf("(imagecov.c)像素均值计算完成\n");
	//所有数据减去均值
	imagdata->data = (cBYTE*)malloc(sizeof(cBYTE) * myraw->rawSize);

//#pragma omp parallel for num_threads(numProcs)
#pragma omp parallel for default(shared) num_threads(numProcs)
	for (i = 0; i < myraw->imgSize; ++i) {
		for (int j = 0; j < myraw->Dim; ++j) {
			imagdata->data[j * myraw->imgSize + i] = myraw->data[j * myraw->imgSize + i] - imagmean->data[i];
		}
	}
	imagdata->Col = myraw->Col;
	imagdata->Row = myraw->Row;
	imagdata->Dim = myraw->Dim;
	imagdata->rawSize = myraw->rawSize;
	imagdata->imgSize = myraw->imgSize;
	//printf("(imagecov.c)像素数据准备完成\n");
	//计算协方差

	// 计时结束
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def imagecov：数据预处理. 时间：%fs\n", duration);

}
