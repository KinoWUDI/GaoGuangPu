#include"savedata.h"
#include"featurevector.h"
#include <omp.h>
//�����м����ݣ�������־

bool saveCovBYTEtofile(covBYTE* cov, char* filename, int size) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("�����ļ�ʧ��/n");
		return false;
	}
	for (int i = 0; i < size; ++i) {
		fwrite(&cov[i], sizeof(covBYTE), 1, fp);

	}
	fclose(fp);
	printf("(saveData.c)�����ļ��ɹ�\n");
	return true;
}
covBYTE* readCovBYTEfromfile(char* dir, int datasize) {
	FILE* fp = fopen(dir, "rb");
	if (fp == NULL) {
		printf("��ȡ�ļ�ʧ��/n");
		return false;
	}
	covBYTE* rcov = (covBYTE*)malloc(sizeof(covBYTE) * datasize);

	for (int i = 0; i < datasize; ++i) {

		fread(&rcov[i], sizeof(covBYTE), 1, fp);
	}


	fclose(fp);
	printf("���ļ��ж�ȡ�������ݳɹ�\n");
	return rcov;
}
//����ֵ����K����ά���ͼ�����ѹ��д��һ���ļ�
bool imagout(Image* ImagMean, Matrix* K, Imagdata* imagdata, char* filename) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("�����ļ�ʧ��/n");
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
	printf("(saveData.c)ѹ��ͼ�񱣴�ɹ�\n");
	return true;
}
bool imagoutjustpic(Imagdata* imagdata, char* filename) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("�����ļ�ʧ��/n");
		return false;
	}

	for (int i = 0; i < imagdata->rawSize; ++i) {
		fwrite(&imagdata->data[i], sizeof(cBYTE), 1, fp);
	}
	fclose(fp);
	printf("(saveData.c)ѹ��ͼ�񱣴�ɹ�\n");
	return true;
}

bool savemean(Image* imagdata, const char filename[]) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("�����ļ�ʧ��/n");
		return false;
	}
	int i;
	for (i = 0; i < imagdata->Col * imagdata->Row; ++i) {
		fwrite(&imagdata->data[i], sizeof(rBYTE), 1, fp);
	}
	fclose(fp);
	//printf("(saveData.c)��ֵ����ɹ�\n");
	return true;
}
bool saveK(Matrix* K, const char filename[]) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("�����ļ�ʧ��/n");
		return false;
	}
	int i;
	for (i = 0; i < K->column * K->row; ++i) {
		fwrite(&K->data[i], sizeof(VBYTE), 1, fp);
	}
	fclose(fp);
	//printf("(saveData.c)K����ɹ�\n");
	return true;
}