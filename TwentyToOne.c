#include "TwentyToOne.h"
#include <windows.h>
#include <time.h>

/*
cmd 调用RAR命令
压缩命令：rar a -r -ep1 d:\\all.hyq d:\\1\\
解压命令：rar x -y -o d:\\all.hyq d:\\2
*/
void twentyToOne(char workPath[], char inputPath[], char outputPath[]) {

	//printf("(twentyToOne.c)文件合并过程开始\n");

	//char cmdIn_bpg = inputPath;
	//char cmdOut_bpg = outputPath;

	// 计时开始
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();

	char hebingPath[500];
	strcpy(hebingPath, workPath);

	char rarPath[100] = "WinRAR\\rar a -r -ep1 "; //-ep1 向前退一层文件夹
	strcat(hebingPath, rarPath);

	strcat(hebingPath, outputPath);

	strcat(hebingPath, " ");

	strcat(hebingPath, inputPath);

	//CMD执行命令
	system(hebingPath);

	// 计时结束
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def twentyToOne：合并单文件. 时间：%fs\n", duration);

	//printf("(twentyToOne.c)文件合并过程完成\n");

	//printf("(twentyToOne.c)已生成hyq文件\n");

}