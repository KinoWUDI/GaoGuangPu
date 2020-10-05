#include"Readimage.h"
#include <time.h>
/************************************************************************
函数功能：根据路径读取高光谱图像数据
输入：路径字符串，图像存储数组名
输出：图像数据指针/*************************/

bool Readimage(RawImag* raw,char *path, int row,int col, int start_dim, int end_dim)//Len = 680,Wid=512,Dim = 224
{
	// 计时开始
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();

	int dim = end_dim - start_dim + 1;
	int rawSize = row * col * dim;
	FILE* imageFileP;
	if ((imageFileP = fopen(path, "rb")) == NULL){
		printf("未读取到图像\n");
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

	// 计时结束
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;

	printf("def Readimage：读取图像. 时间：%fs\n", duration);

	return true;
}