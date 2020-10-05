#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>

#define height 680
#define width 256
#define depth 224
int picsize = 174080;

typedef unsigned short BYTE;//���ֽ� 0~65536
BYTE rawFile[depth][height][width];//��������2Mʱ������ʼ���ᵽȫ��

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
	//printf("��ֵΪ%f\n", equd);
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
		//�˴��뿴��FILEָ�����͵Ľṹ��
		printf("ͼ���ȡ�ɹ�\n");
	}

	//rawBitP = (BYTE*)malloc(depth*picsize*sizeof(BYTE));//�˴�����ԭ����Ȼ����ѭ����Ϊʲô���СΪ����ͼ���С��
	rawBitP = (BYTE*)malloc(sizeof(BYTE));
	BYTE rawtemp;
	for (i = 0; i < width; i++)//������Ƶ���������ݴ���width
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
	fclose(rawFileP);//�����ʱ�����е��˴�����Ҫ7s��

	printf("%d\n", (int)rawFile[0][0][0]);
	printf("%d\n", (int)&rawFile[0][0][0]);
	/*
	rawFile ��ά�����ʽ�������ȡ����raw���ݸ�ʽ��
	*/

	//����Э�������
	float  covInRaw[depth][depth];
	BYTE(*picRawP1)[174080] =rawFile;
	BYTE(*picRawP2)[174080]=rawFile;

	//printf("��ʼ����Э�������\n");
	for (m = 0; m < depth; m++)
	{
		
		//printf("��%d��\n", m);
		for (n = m; n < depth; n++)
		{
			covInRaw[m][n] = covaArray(picRawP1[m], picRawP2[n]);
			covInRaw[n][m] = covInRaw[m][n];
		}
	}
	printf("����Ϊ��������\n");
	//printf("%d\n", (int)rawFile[0][0][1]);
	//printf("%d\n", (int)covInRaw[0][0]);
	//�鿴Э����������ݣ�������python��������һ��--------------------------------
	for (int i = 0; i < depth; i++)
	{
		for (int j = 0; j < depth; j++)
		{
			printf("%f,", covInRaw[i][j]);
		}
		printf("\n");
	}
	//printf("%d\n", (int)(**picRawP1));//��ָ֤�봫��������ȷ��
	//printf("Эty       ����%f\n", covInRaw[0][0]);//��֤���ص�Э��������
	getchar();
	return 0;
}