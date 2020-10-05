#include "OneToTwenty.h"
#include <windows.h>

/*
cmd 调用RAR命令
压缩命令：rar a -r -ep1 d:\\all.hyq d:\\1\\
解压命令：rar x -y -o d:\\all.hyq d:\\2
*/
void oneToTwenty(char workPath[], char inputPath[], char outputPath[]) {

	printf("(oneToTwenty.c)文件拆分过程开始\n");

	//char cmdIn_bpg = inputPath;
	//char cmdOut_bpg = outputPath;
	char chaifenPath[500];

	strcpy(chaifenPath, workPath);

	char rarPath[50] = "WinRAR\\rar x -y -o "; //-ep1 向前退一层文件夹

	strcat(chaifenPath, rarPath);

	strcat(chaifenPath, inputPath);

	strcat(chaifenPath, " ");

	strcat(chaifenPath, outputPath);

	//CMD执行命令
	system(chaifenPath);


	printf("(oneToTwenty.c)文件拆分过程完成\n");

	printf("(oneToTwenty.c)已生成BPG文件\n");

}