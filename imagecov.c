#include"imagecov.h"
#include <omp.h>

#include<time.h>

void imagecov(RawImag* myraw, Imagdata* imagdata, Image* imagmean) {

	//clock_t start, stop; //clock_tΪclock()�������صı�������
	//double duration;

	// ��ʱ��ʼ
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();

	//��ȡ����߳���
	int numProcs = omp_get_num_procs();

	//�����ֵ
	long count, count1;
	long long count2;
	covBYTE equ1, equ2;

	imagmean->data = (rBYTE*)malloc(sizeof(rBYTE) * myraw->imgSize);
	int i = 0;
// #pragma omp parallel for private��iindex��shared��array��
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
	//printf("(imagecov.c)���ؾ�ֵ�������\n");
	//�������ݼ�ȥ��ֵ
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
	//printf("(imagecov.c)��������׼�����\n");
	//����Э����

	// ��ʱ����
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def imagecov������Ԥ����. ʱ�䣺%fs\n", duration);

}
