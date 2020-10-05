#include"Readimage.h"
#include <time.h>
/************************************************************************
�������ܣ�����·����ȡ�߹���ͼ������
���룺·���ַ�����ͼ��洢������
�����ͼ������ָ��/*************************/

bool Readimage(RawImag* raw,char *path, int row,int col, int start_dim, int end_dim)//Len = 680,Wid=512,Dim = 224
{
	// ��ʱ��ʼ
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();

	int dim = end_dim - start_dim + 1;
	int rawSize = row * col * dim;
	FILE* imageFileP;
	if ((imageFileP = fopen(path, "rb")) == NULL){
		printf("δ��ȡ��ͼ��\n");
		return false;
	}
	raw->data = (rBYTE*)malloc(sizeof(rBYTE)*rawSize);

	fseek(imageFileP, 2 * row * col * (start_dim - 1), SEEK_SET);
	fread(raw->data, 2, rawSize, imageFileP);

	raw->Col = col;
	raw->Row = row;
	raw->Dim = dim;
	raw->rawSize = row * col * dim;
	raw->imgSize = row * col;
	fclose(imageFileP);

	// ��ʱ����
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;

	printf("def Readimage����ȡͼ��. ʱ�䣺%fs\n", duration);

	return true;
}