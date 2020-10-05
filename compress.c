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
	//printf("(compress.c)降维矩阵已生成\n");
}
//---------------------------------------------------------
//此过程会消耗18秒,并发处理后为5-18秒,改成单张png之后时间为0.80s
void retimagedata(Imagdata* imagdata, Matrix* K) {

	// 计时开始
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
	//printf("(compress.c)图像压缩完成\n");

	//printf("图像压缩部分，优化后运行时间为：%f 秒\n", duration);//5.363s

	// 计时结束
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def retimagedata：降维压缩. 时间：%fs\n", duration);
//---------------------------------------------------------
}
