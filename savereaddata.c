#include"savereaddata.h"

void imagin(Image * imagmean, Matrix *K, Imagdata *imagdata, char *filename) {
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("读取文件失败/n");
		return false;
	}
	imagmean->data = (RBYTE*)malloc(sizeof(RBYTE)*imagmean->Col*imagmean->Row);
	K->data = (VBYTE*)malloc(sizeof(VBYTE)*K->column*K->row);
	imagdata->data = (CBYTE*)malloc(sizeof(CBYTE)*imagdata->rawSize);
	
	fread(imagmean->data, sizeof(RBYTE), imagmean->Col*imagmean->Row, fp);
	fread(K->data, sizeof(VBYTE), K->column*K->row, fp);
	fread(imagdata->data, sizeof(CBYTE), imagdata->rawSize, fp);
	fclose(fp);
	printf("从文件中读取到的数据成功\n");
}

void imagre(Imagdata *imagdata, char *filename) {
	FILE *fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("读取文件失败/n");
		return false;
	}
	fwrite(imagdata->data, sizeof(RBYTE), imagdata->rawSize, fp);
	fclose(fp);
	printf("恢复图像成功\n");
}

void readChardat(unsigned char* data,char *filename,int size) {
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("读取文件失败\n");
		getchar();
		return 0;
	}
	fread(data, sizeof(unsigned char), size, fp);
	fclose(fp);
}

void readK(Matrix* K, char *filename) {
	VBYTE *data = (VBYTE*)malloc(sizeof(VBYTE)*K->column *K->row);
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("读取文件失败\n");
		getchar();
		return 0;
	}
	fread(data, sizeof(VBYTE), K->column * K->row, fp);
	K->data = data;
	fclose(fp);
}

void readmean(Image* mean, char *filename) {
	RBYTE *data = (RBYTE*)malloc(sizeof(RBYTE)*mean->Col *mean->Row);
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("读取文件失败\n");
		getchar();
		return 0;
	}
	fread(data, sizeof(RBYTE), mean->Col* mean->Row, fp);
	mean->data = data;
	fclose(fp);
}
