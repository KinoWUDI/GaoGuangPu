#include "OneToTwenty.h"
#include <windows.h>

/*
cmd ����RAR����
ѹ�����rar a -r -ep1 d:\\all.hyq d:\\1\\
��ѹ���rar x -y -o d:\\all.hyq d:\\2
*/
void oneToTwenty(char workPath[], char inputPath[], char outputPath[]) {

	printf("(oneToTwenty.c)�ļ���ֹ��̿�ʼ\n");

	//char cmdIn_bpg = inputPath;
	//char cmdOut_bpg = outputPath;
	char chaifenPath[500];

	strcpy(chaifenPath, workPath);

	char rarPath[50] = "WinRAR\\rar x -y -o "; //-ep1 ��ǰ��һ���ļ���

	strcat(chaifenPath, rarPath);

	strcat(chaifenPath, inputPath);

	strcat(chaifenPath, " ");

	strcat(chaifenPath, outputPath);

	//CMDִ������
	system(chaifenPath);


	printf("(oneToTwenty.c)�ļ���ֹ������\n");

	printf("(oneToTwenty.c)������BPG�ļ�\n");

}