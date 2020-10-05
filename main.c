#include <stdio.h>
#include <windows.h>
#include "savereaddata.h"
#include "reimage.h"
#include "savereaddata.h"

//#include "OtoN.h"
#include "OneToTwenty.h"


#include "getopt.h"

#ifndef WIN32

#else
#include "getopt.h"
#endif

#define USAGE_STRING "Usage: %s --input original_samples --output compressedImage"

struct option options[] = {
	{"input", 1, NULL, 1},
	{"output", 1, NULL, 2},
	{"start_dim", 1, NULL, 3},
	{"end_dim", 1, NULL, 4},
	{NULL, 0, NULL, 0}
};

int main(int argc, char* argv[])
{
	/*
	封装EXE代码
	*/

	char samples_file[128];
	char out_file[128];
	int start_dim = 1;
	int end_dim = 224;
	int foundOpt = 0;
	do {

		foundOpt = getopt_long(argc, argv, "", options, NULL);
		switch (foundOpt) {
		case 1:
			strcpy(samples_file, optarg);
			break;
		case 2:
			strcpy(out_file, optarg);
			break;
		case 3:
			start_dim = (unsigned int)atoi(optarg);
		case 4:
			end_dim = (unsigned int)atoi(optarg);
		case -1:
			//结束
			break;
		case '?':
		default:
			fprintf(stderr, "\nError in the program command line!!\n\n");
			fprintf(stderr, USAGE_STRING, argv[0]);
			return -1;
			break;
		}
	} while (foundOpt >= 0);


	//获取当前EXE的运行路径
//argv[0]是当前运行程序的全路径名
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char workPath[100];

	_splitpath(argv[0], drive, dir, fname, ext);//drive:驱动盘	dir:路径 fname:文件名 ext:后缀
	strcpy(workPath, drive);
	strcat(workPath, dir);// workPath储存arg[0]所输入的工作路径-"D:\xx\xx\"


	Image imagemean;//均值
	//Image imagemean1;//均值1
	//Image imagemean2;//均值2


	Matrix K;//变换矩阵
	Matrix K1;//变换矩阵1
	Matrix K2;//变换矩阵2


	Imagdata imagdata;//归一化图像数据
	Imagdata imagdata1;//归一化图像数据1
	Imagdata imagdata2;//归一化图像数据2



	RawImag resRaw;//raw文件数据

	imagemean.Col = 512;
	imagemean.Row = 680;

	K.column = end_dim - start_dim + 1;
	K.row = 2;
	K1.column = (end_dim - start_dim) / 2 + 1;
	K1.row = 1;
	K2.column = (end_dim - start_dim) / 2 + 1;
	K2.row = 1;

	imagdata.Col = 512;
	imagdata.Row = 680;
	imagdata.Dim = 2;
	imagdata.imgSize = imagdata.Col * imagdata.Row;
	imagdata.rawSize = imagdata.imgSize * imagdata.Dim;

	imagdata1.Col = 512;
	imagdata1.Row = 680;
	imagdata1.Dim = 1;
	imagdata1.imgSize = 512 * 680;
	imagdata1.rawSize = 512 * 680 * 1;

	imagdata2.Col = 512;
	imagdata2.Row = 680;
	imagdata2.Dim = 1;
	imagdata2.imgSize = 512 * 680;
	imagdata2.rawSize = 512 * 680 * 1;
;

	//1个压缩文件解压成22个文件
	//rBPG();

	//4.26暂时利用RAR方式进行文件拆分操作
	//char inputChaifen[100];
	//char outputChaifen[100];

	//strcpy(inputChaifen, "..\\..\\final.hyq");
	//strcpy(outputChaifen, "..\\in");

	//命令行调用
	char inputChaifen[100];
	char outputChaifen[100];

	strcpy(inputChaifen, samples_file);//输入hyq压缩文件路径

	strcpy(outputChaifen, workPath);
	strcat(outputChaifen, "tempRBPG\\");//输出BPG,K,MEAN文件路径

	oneToTwenty(workPath, inputChaifen, outputChaifen);

	printf("BPG转换为PNG过程开始...\n");

	//---------------------------------------
	//读取bpg转png

	char num[4];

	for (int i = 0; i < 2; ++i) {
		//构建命令字符串
		//char comSta[100] = "..\\..\\bpg\\bpgdec -o temp\\";

		char rbpgPath[500];
		strcpy(rbpgPath, workPath);

		//命令行
		char comSta[100] = "bpg\\bpgdec -o ";
		strcat(rbpgPath, comSta);

		char tempPngPath[100];
		strcpy(tempPngPath, workPath);
		strcat(tempPngPath, "temp\\");

		strcat(rbpgPath, tempPngPath);

		_itoa(i, num, 10);
		strcat(rbpgPath, num);//保存名序号
		//strcat(comSta, "r.png  ..\\..\\KLT\\out\\");//保存文件路径	4.25以前版本
		strcat(rbpgPath, "r.png ");//保存文件路径+全称

		char tempRBpgPath[100];
		strcpy(tempRBpgPath, workPath);
		strcat(tempRBpgPath, "tempRBPG\\");

		strcat(rbpgPath, tempRBpgPath);

		strcat(rbpgPath, num);//读取文件名序号
		strcat(rbpgPath, "bpg.bpg");//读取文件名全称
		//执行命令
		system(rbpgPath);
		//system("..\\..\\bpg\\bpgdec -o ..\\temp\\18r.png  ..\\..\\KLT\\out\\18bpg.bpg");
	}

	printf("BPG转换为PNG过程完成\n");
	//---------------------------------------
	/*解码PNG*/
	//---------------------------------------

	printf("解码PNG过程开始...\n");
	//读取
	//system("..\\p2b.exe");

	//更改当前工作路径，随输入的atg[0]而定
	char buf[1000];
	strcpy(buf, workPath);

	SetCurrentDirectory(buf);  //设置工作路径

	//GetCurrentDirectory(1000, buf);

	//命令行
	char p2bPath[100];
	strcpy(p2bPath, workPath);
	strcat(p2bPath, "p2b.exe");

	system(p2bPath);
	/*只能读取当前目录下的temp文件夹中文件名为“0r.png”类似文件，temp文件夹不能放置其他任何文件
	生成数据文件子到当前目录文件夹“temp0”中*/
	unsigned char* tempdata8= (unsigned char*)malloc(sizeof(unsigned char)* 680*512*2);//不在堆栈中申请空间，在堆中申请

	for (int i = 0; i < 2; i++) {
		_itoa(i, num, 10);

		char outname[100];
		strcpy(outname, workPath);

		//命令行
		char comSta[20] = "temp0\\rdata";
		strcat(outname, comSta);

		strcat(outname, num);
		strcat(outname, ".dat");
		readChardat((tempdata8+i*680*512),outname, 680 * 512);
	}

	//CBYTE* tempdataB = (CBYTE*)malloc(sizeof(CBYTE) * imagdata.rawSize);
	//for (int i = 0; i < imagdata.rawSize / 2; i++) {
	//	//tempdataB[i] = (tempdata8[i]/256.0)*72327 - 10447; // 5张PNG
	//	tempdataB[i] = (tempdata8[i] / 256.0) * 65029 - 3149;// 1张PNG
	//}

	/*
	分两块
	*/

	CBYTE* tempdataB = (CBYTE*)malloc(sizeof(CBYTE) * imagdata.rawSize);
	for (int i = 0; i < imagdata.rawSize / 2; i++) {
		//tempdataB[i] = (tempdata8[i]/256.0)*72327 - 10447; // 5张PNG
		tempdataB[i] = (tempdata8[i] / 256.0) * 36991 - 457;// 1张PNG
	}

	for (int i = imagdata.rawSize / 2; i < imagdata.rawSize; i++) {
		//tempdataB[i] = (tempdata8[i]/256.0)*72327 - 10447; // 5张PNG
		tempdataB[i] = (tempdata8[i] / 256.0) * 44164 - 173;// 1张PNG
	}


	imagdata.data = tempdataB;

	//imagdata.data = tempdataB;
	free(tempdata8);

	printf("解码PNG过程完成\n");

	//readK(&K,"..\\..\\KLT\\out\\K.dat");//  4.25以前版本
	//readmean(&imagemean, "..\\..\\KLT\\out\\mean.dat");//	4.25以前版本

	//readK(&K, "..\\in\\K.dat");
	//readmean(&imagemean, "..\\in\\mean.dat");

	//命令行
	char K1Path[100];
	strcpy(K1Path, workPath);
	strcat(K1Path, "tempRBPG\\K1.dat");

	char K2Path[100];
	strcpy(K2Path, workPath);
	strcat(K2Path, "tempRBPG\\K2.dat");


	readK(&K1, K1Path);
	readK(&K2, K2Path);

	VBYTE* K_data = (VBYTE*)malloc(sizeof(VBYTE) * K.column * K.row);
	memcpy(K_data, K1.data, K1.column* K1.row * sizeof(VBYTE));
	memcpy(K_data + K1.column * K1.row, K2.data, K2.column* K2.row * sizeof(VBYTE));

	K.data = K_data;

	char MeanPath[100];
	strcpy(MeanPath, workPath);
	strcat(MeanPath, "tempRBPG\\mean.dat");

	//readK(&K, KPath);
	readmean(&imagemean, MeanPath);

	printf("读取K和MEAN文件成功\n");

	reimage(&imagemean, &K, &imagdata,&resRaw);//重构图像
	//imagre(&resRaw, "out\\restore.raw");//储存为raw文件

	//命令行
	imagre(&resRaw, out_file);//储存为raw文件

	getchar();

	return 0;

}