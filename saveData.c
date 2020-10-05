#include"savedata.h"
#include"featurevector.h"
#include <omp.h>
//保存中间数据，保存日志

bool saveCovBYTEtofile(covBYTE* cov, char* filename, int size) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("保存文件失败/n");
		return false;
	}
	for (int i = 0; i < size; ++i) {
		fwrite(&cov[i], sizeof(covBYTE), 1, fp);

	}
	fclose(fp);
	printf("(saveData.c)保存文件成功\n");
	return true;
}
covBYTE* readCovBYTEfromfile(char* dir, int datasize) {
	FILE* fp = fopen(dir, "rb");
	if (fp == NULL) {
		printf("读取文件失败/n");
		return false;
	}
	covBYTE* rcov = (covBYTE*)malloc(sizeof(covBYTE) * datasize);

	for (int i = 0; i < datasize; ++i) {

		fread(&rcov[i], sizeof(covBYTE), 1, fp);
	}


	fclose(fp);
	printf("从文件中读取到的数据成功\n");
	return rcov;
}
//将均值矩阵、K、降维后的图像矩阵压缩写到一个文件
bool imagout(Image* ImagMean, Matrix* K, Imagdata* imagdata, char* filename) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("保存文件失败/n");
		return false;
	}

	for (int i = 0; i < ImagMean->Row * ImagMean->Col; ++i) {
		fwrite(&ImagMean->data[i], sizeof(rBYTE), 1, fp);
	}

	for (int i = 0; i < K->column * K->row; ++i) {
		fwrite(&K->data[i], sizeof(VBYTE), 1, fp);
	}
	for (int i = 0; i < imagdata->rawSize; ++i) {
		fwrite(&imagdata->data[i], sizeof(cBYTE), 1, fp);
	}
	fclose(fp);
	printf("(saveData.c)压缩图像保存成功\n");
	return true;
}
bool imagoutjustpic(Imagdata* imagdata, char* filename) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("保存文件失败/n");
		return false;
	}

	for (int i = 0; i < imagdata->rawSize; ++i) {
		fwrite(&imagdata->data[i], sizeof(cBYTE), 1, fp);
	}
	fclose(fp);
	printf("(saveData.c)压缩图像保存成功\n");
	return true;
}

bool savemean(Image* imagdata, const char filename[]) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("保存文件失败/n");
		return false;
	}
	int i;
	for (i = 0; i < imagdata->Col * imagdata->Row; ++i) {
		fwrite(&imagdata->data[i], sizeof(rBYTE), 1, fp);
	}
	fclose(fp);
	//printf("(saveData.c)均值保存成功\n");
	return true;
}
bool saveK(Matrix* K, const char filename[]) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("保存文件失败/n");
		return false;
	}
	int i;
	for (i = 0; i < K->column * K->row; ++i) {
		fwrite(&K->data[i], sizeof(VBYTE), 1, fp);
	}
	fclose(fp);
	//printf("(saveData.c)K保存成功\n");
	return true;
}