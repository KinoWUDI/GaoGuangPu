#pragma once
#include<stdbool.h>
#include<stdio.h>
#include<malloc.h>



typedef unsigned short RBYTE;
typedef int CBYTE;
typedef float VBYTE;
typedef struct //rawͼ��ṹ��
{
	int Row;
	int Col;
	int Dim;
	int rawSize;
	int imgSize;
	RBYTE *data;
}RawImag;

typedef struct {
	int Row;
	int Col;
	RBYTE *data;
}Image;


typedef struct //rawͼ�����ֵ
{
	int Row;
	int Col;
	int Dim;
	int rawSize;
	int imgSize;
	CBYTE *data;
}Imagdata;

typedef struct //����ṹ��
{
	int row;
	int column;
	VBYTE *data;
}Matrix;

void imagin(Image * imagmean, Matrix *K, Imagdata *imagdata, char *filename);
void readChardat(unsigned char* data, char *filename, int size);
void imagre(Imagdata* imagdata, char* filename);
void readK(Matrix* K, char* filename);
void readmean(Image* mean, char* filename);
