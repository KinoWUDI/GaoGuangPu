#include "bpg.h"
#include <omp.h>
#include <windows.h>
#include <time.h>

void bpg(char workPath[], char inputPath[], char outputPath[], int ratio, int png_num) {

	clock_t bpg_start, bpg_stop;
	double bpg_duration;

	//获取最大线程数
	int numProcs = omp_get_num_procs();

	bpg_start = clock();

	//char cmdIn_bpg = inputPath;
	//char cmdOut_bpg = outputPath;
	char saveNum[4];
	char quaNum[4];
	char bpgencPath[100] = "bpg\\bpgenc -o ";

	//2-256的压缩比对应 -q参数的0-51
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
	//压缩比参数
	int imagQua = ratio;// -q 对应的是压缩率，q的参数越大，压缩率越大
	_itoa(imagQua, quaNum, 10); //imagQua int 转 char

	//SYSTEMTIME sys;
	//GetLocalTime(&sys);

	int i;

	//printf("(bpg.c)bpg编码过程开始\n");

	for (i = 0; i < png_num * 2; ++i)
	{
		_itoa(i, saveNum, 10);
		//构建命令字符串
		//char bpgPath[100] = "..\\..\\bpg\\bpgenc -o ";

		//命令行 同目录调用
		char bpgPath[500];
		strcpy(bpgPath, workPath);

		strcat(bpgPath, bpgencPath);//bpgexe和输出目录

		strcat(bpgPath, outputPath);

		strcat(bpgPath, saveNum);//保存名序号

		strcat(bpgPath, "bpg.bpg -c rgb -m 1 -b 8 -q ");//保存名全称+图像类型+质量

		strcat(bpgPath, quaNum);//质量参数

		strcat(bpgPath, " ");

		strcat(bpgPath, inputPath);//输入目录

		strcat(bpgPath, saveNum);//读取文件名序号

		strcat(bpgPath, ".png");//读取文件名全称

		//执行命令
		system(bpgPath);

		//system("..\\..\\bpg\\bpgenc -o ..\\temp\\18bpg.bpg -c rgb -q 10  ..\\out\\18.png");
		//printf("生成%d,质量为%d\n", i, imagQua);

		//printf("当前第%d次系统时间：%02d:%02d:%02d.%03d \n", i, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);

	}
	//printf("(bpg.c)bpg编码过程完成\n");
	bpg_stop = clock();
	bpg_duration = (double)(bpg_stop - bpg_start) / CLK_TCK; //CLK_TCK为clock()函数的时间单位
	printf("def bpg：Bpg压缩. 时间：%fs\n", bpg_duration);

}