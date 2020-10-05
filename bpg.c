#include "bpg.h"
#include <omp.h>
#include <windows.h>
#include <time.h>

void bpg(char workPath[], char inputPath[], char outputPath[], int ratio, int png_num) {

	clock_t bpg_start, bpg_stop;
	double bpg_duration;

	//��ȡ����߳���
	int numProcs = omp_get_num_procs();

	bpg_start = clock();

	//char cmdIn_bpg = inputPath;
	//char cmdOut_bpg = outputPath;
	char saveNum[4];
	char quaNum[4];
	char bpgencPath[100] = "bpg\\bpgenc -o ";

	//2-256��ѹ���ȶ�Ӧ -q������0-51
	if (ratio <= 4)
	{
		ratio = 0;
	}

	else if (ratio == 256)
	{
		ratio = 51;
	}
	else
	{
		ratio = ratio / 5;
	}
	//ѹ���Ȳ���
	int imagQua = ratio;// -q ��Ӧ����ѹ���ʣ�q�Ĳ���Խ��ѹ����Խ��
	_itoa(imagQua, quaNum, 10); //imagQua int ת char

	//SYSTEMTIME sys;
	//GetLocalTime(&sys);

	int i;

	//printf("(bpg.c)bpg������̿�ʼ\n");

	for (i = 0; i < png_num * 2; ++i)
	{
		_itoa(i, saveNum, 10);
		//���������ַ���
		//char bpgPath[100] = "..\\..\\bpg\\bpgenc -o ";

		//������ ͬĿ¼����
		char bpgPath[500];
		strcpy(bpgPath, workPath);

		strcat(bpgPath, bpgencPath);//bpgexe�����Ŀ¼

		strcat(bpgPath, outputPath);

		strcat(bpgPath, saveNum);//���������

		strcat(bpgPath, "bpg.bpg -c rgb -m 1 -b 8 -q ");//������ȫ��+ͼ������+����

		strcat(bpgPath, quaNum);//��������

		strcat(bpgPath, " ");

		strcat(bpgPath, inputPath);//����Ŀ¼

		strcat(bpgPath, saveNum);//��ȡ�ļ������

		strcat(bpgPath, ".png");//��ȡ�ļ���ȫ��

		//ִ������
		system(bpgPath);

		//system("..\\..\\bpg\\bpgenc -o ..\\temp\\18bpg.bpg -c rgb -q 10  ..\\out\\18.png");
		//printf("����%d,����Ϊ%d\n", i, imagQua);

		//printf("��ǰ��%d��ϵͳʱ�䣺%02d:%02d:%02d.%03d \n", i, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);

	}
	//printf("(bpg.c)bpg����������\n");
	bpg_stop = clock();
	bpg_duration = (double)(bpg_stop - bpg_start) / CLK_TCK; //CLK_TCKΪclock()������ʱ�䵥λ
	printf("def bpg��Bpgѹ��. ʱ�䣺%fs\n", bpg_duration);

}