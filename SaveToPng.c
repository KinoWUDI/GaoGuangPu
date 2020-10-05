#include "SaveToPng.h"
#include <stdio.h>
#include <omp.h>
#include<time.h>

//保存图像为png文件
void savetopng1(Imagdata* imagdata, char* path)
{
	// 计时开始
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();

	char num[4];
	rBYTE data;
	unsigned char rgb[512 * 680 * 3], * p = rgb;

	for (int i = 0; i < imagdata->Dim; ++i)
	{
		// printf("%d\n", imagdata->Dim);
		_itoa(i, num, 10);
		char savepath[100];
		strcpy(savepath, path);

		strcat(savepath, num);
		strcat(savepath, ".png");
		FILE* fp = fopen(savepath, "wb");
		int j = 0;
		for (j = 0; j < imagdata->imgSize; ++j) {
			//printf("%d ", imagdata.data[i*imagdata.imgSize + j]);
			data = imagdata->data[i * imagdata->imgSize + j];
			*p++ = (unsigned char)0;
			*p++ = (unsigned char)0;
			*p++ = (unsigned char)data;
		}
		svpng(fp, imagdata->Row, imagdata->Col, rgb, 0);
		fclose(fp);
		p = rgb;//返回原始地址
	}

	//printf("(savetopng.c)保存PNG完成..\n");

	// 计时结束
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def savetopng：保存PNG. 时间：%fs\n", duration);
}

void savetopng2(Imagdata* imagdata, char* path)
{
	// 计时开始
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();

	char num[4];
	rBYTE data;
	unsigned char rgb[512 * 680 * 3], * p = rgb;

	for (int i = 1; i < 2; ++i)
	{
		// printf("%d\n", imagdata->Dim);
		_itoa(i, num, 10);
		char savepath[100];
		strcpy(savepath, path);

		strcat(savepath, num);
		strcat(savepath, ".png");
		FILE* fp = fopen(savepath, "wb");
		int j = 0;
		for (j = 0; j < imagdata->imgSize; ++j) {
			//printf("%d ", imagdata.data[i*imagdata.imgSize + j]);
			// 此处因分多块而改动
			//data = imagdata->data[i * imagdata->imgSize + j];
			data = imagdata->data[0 * imagdata->imgSize + j];
			*p++ = (unsigned char)0;
			*p++ = (unsigned char)0;
			*p++ = (unsigned char)data;
		}
		svpng(fp, imagdata->Row, imagdata->Col, rgb, 0);
		fclose(fp);
		p = rgb;//返回原始地址
	}

	//printf("(savetopng.c)保存PNG完成..\n");

	// 计时结束
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def savetopng：保存PNG. 时间：%fs\n", duration);
}
