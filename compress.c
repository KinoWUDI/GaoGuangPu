#include"compress.h"
#include <omp.h>
#include<time.h>

void reduceDim(Matrix* K, int n) {

	VBYTE* newK = (VBYTE*)malloc(sizeof(VBYTE) * K->column * n);
	for (int i = 0; i < K->column * n; i++) {
		newK[i] = K->data[i];
	}
	free(K->data);
	K->data = newK;
	K->row = n;
	//printf("(compress.c)��ά����������\n");
}
//---------------------------------------------------------
//�˹��̻�����18��,���������Ϊ5-18��,�ĳɵ���png֮��ʱ��Ϊ0.80s
void retimagedata(Imagdata* imagdata, Matrix* K) {

	// ��ʱ��ʼ
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();

	int h, l; cBYTE temp;
	cBYTE* newimag = (cBYTE*)malloc(sizeof(cBYTE) * imagdata->imgSize * K->row);

	int i;
	for (i = 0; i < imagdata->imgSize * K->row; ++i) {
		h = i / imagdata->imgSize;
		l = i % imagdata->imgSize;
		temp = 0;
		int j;
		for (j = 0; j < K->column; j++) {
			temp += K->data[h * K->column + j] * (-1) * imagdata->data[j * imagdata->imgSize + l];
		}
		newimag[i] = temp;
	}
	free(imagdata->data);
	imagdata->data = newimag;
	imagdata->Dim = K->row;
	imagdata->rawSize = imagdata->imgSize * K->row;
	//printf("(compress.c)ͼ��ѹ�����\n");

	//printf("ͼ��ѹ�����֣��Ż�������ʱ��Ϊ��%f ��\n", duration);//5.363s

	// ��ʱ����
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def retimagedata����άѹ��. ʱ�䣺%fs\n", duration);
//---------------------------------------------------------
}
