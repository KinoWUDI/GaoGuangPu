#include "TwentyToOne.h"
#include <windows.h>
#include <time.h>

/*
cmd ����RAR����
ѹ�����rar a -r -ep1 d:\\all.hyq d:\\1\\
��ѹ���rar x -y -o d:\\all.hyq d:\\2
*/
void twentyToOne(char workPath[], char inputPath[], char outputPath[]) {

	//printf("(twentyToOne.c)�ļ��ϲ����̿�ʼ\n");

	//char cmdIn_bpg = inputPath;
	//char cmdOut_bpg = outputPath;

	// ��ʱ��ʼ
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();

	char hebingPath[500];
	strcpy(hebingPath, workPath);

	char rarPath[100] = "WinRAR\\rar a -r -ep1 "; //-ep1 ��ǰ��һ���ļ���
	strcat(hebingPath, rarPath);

	strcat(hebingPath, outputPath);

	strcat(hebingPath, " ");

	strcat(hebingPath, inputPath);

	//CMDִ������
	system(hebingPath);

	// ��ʱ����
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def twentyToOne���ϲ����ļ�. ʱ�䣺%fs\n", duration);

	//printf("(twentyToOne.c)�ļ��ϲ��������\n");

	//printf("(twentyToOne.c)������hyq�ļ�\n");

}