#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>

#define height 680
#define width 256
#define depth 224
int picsize = 174080;

typedef unsigned short BYTE;//两字节 0~65536
BYTE rawFile[depth][height][width];//变量大于2M时，将初始化提到全局

float equArray(BYTE (*arrayP)[174080])
{
	int i;
	int count=0;
	float equd;
	
	for (i = 0; i < picsize; i++)
	{
		//printf("%d,", (*arrayP)[i]);
		count += (*arrayP)[i];
		
	}
	equd = count / picsize;
	//printf("均值为%f\n", equd);
	return equd;
}

float covaArray(BYTE(*arrayP1)[174080], BYTE(*arrayP2)[174080])
{
	int i;
	float count=0,cova;
	float eq1 = equArray(arrayP1);
	float eq2 = equArray(arrayP2);
	for (i = 0; i < picsize; i++)
	{
		count += ((*arrayP1)[i] - eq1)*((*arrayP2)[i] - eq2);
	}
	cova = count / (picsize - 1);
	return cova;
}
int main(void)
{
	FILE* rawFileP;
	BYTE* rawBitP;

	char path[] = "F:\\home\\2019_ggp\\pic\\r.raw";
	int i, j, k,m,n;
	if ((rawFileP = fopen(path, "rb")) == NULL)
	{
		printf("not read");
		return 1;
	}
	else
	{
		/*printf("%d\n",(int)&rawFileP);
		printf("%d\n", (int) (&(rawFileP+1)) );*/
		//此处想看看FILE指针类型的结构；
		printf("图像读取成功\n");
	}

	//rawBitP = (BYTE*)malloc(depth*picsize*sizeof(BYTE));//此处具体原理，既然下有循环，为什么其大小为整个图像大小？
	rawBitP = (BYTE*)malloc(sizeof(BYTE));
	BYTE rawtemp;
	for (i = 0; i < width; i++)//处理逐频带交错数据处理width
	{
		for (j = 0; j < height; j++)//height
		{
			for (k = 0; k < depth; k++)
			{
				if (fread(rawBitP, 2, 1, rawFileP)==1)
				{
					rawtemp = *rawBitP;
					rawFile[k][j][i] = (rawtemp << 8) | (rawtemp >> 8);
					//printf("%d\n", rawFile[k][j][i]);
				}
				
			}
		}
	}
	fclose(rawFileP);//程序计时，运行到此处即需要7s。

	printf("%d\n", (int)rawFile[0][0][0]);
	printf("%d\n", (int)&rawFile[0][0][0]);
	/*
	rawFile 三维数组格式，保存读取到的raw数据格式。
	*/

	//计算协方差矩阵
	float  covInRaw[depth][depth];
	BYTE(*picRawP1)[174080] =rawFile;
	BYTE(*picRawP2)[174080]=rawFile;

	//printf("开始计算协方差矩阵\n");
	for (m = 0; m < depth; m++)
	{
		
		//printf("第%d行\n", m);
		for (n = m; n < depth; n++)
		{
			covInRaw[m][n] = covaArray(picRawP1[m], picRawP2[n]);
			covInRaw[n][m] = covInRaw[m][n];
		}
	}
	printf("以下为测试数据\n");
	//printf("%d\n", (int)rawFile[0][0][1]);
	//printf("%d\n", (int)covInRaw[0][0]);
	//查看协方差矩阵数据，和利用python计算结果不一样--------------------------------
	for (int i = 0; i < depth; i++)
	{
		for (int j = 0; j < depth; j++)
		{
			printf("%f,", covInRaw[i][j]);
		}
		printf("\n");
	}
	//printf("%d\n", (int)(**picRawP1));//验证指针传递数据正确性
	//printf("协ty       方差%f\n", covInRaw[0][0]);//验证返回的协方差数据
	getchar();
	return 0;
}