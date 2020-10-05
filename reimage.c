#include"reimage.h"

void reimage(Image * imagmean, Matrix *K, Imagdata *imagdata, RawImag *resRaw) {
	int h, l,count;
	
	RBYTE*newdata = (RBYTE*)malloc(sizeof(RBYTE)*imagdata->imgSize*K->column);

	for (int i = 0; i < imagdata->imgSize*K->column; ++i) {
		h = i / imagdata->imgSize;
		l = i % imagdata->imgSize;
		count = 0;
		for (int j = 0; j < K->row; ++j) {
			count += K->data[j*K->column + h] * imagdata->data[j*imagdata->imgSize + l];
		}
		newdata[i] = count;
	}
	for (int i = 0; i < imagmean->Col*imagmean->Row; ++i) {//
		for (int j = 0; j < K->column; ++j) {//
			newdata[imagdata->imgSize*j + i] = (RBYTE)imagmean->data[i] - newdata[imagdata->imgSize*j + i] - 2000;
		}
	}
	free(imagdata->data);
	imagdata->data = NULL;
	resRaw ->data = newdata;
	resRaw->Dim = K->column;
	resRaw->Col = imagdata->Col;
	resRaw->Row = imagdata->Row;
	resRaw->imgSize = resRaw->Col*resRaw->Row;
	resRaw->rawSize = resRaw->imgSize*resRaw->Dim;

}