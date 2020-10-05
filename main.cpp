#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include <Windows.h>
#include <direct.h>
#include <string>
#include <omp.h>
#include "svpng.inc"

#include "Readimage.h"
#include "imagecov.h"
#include "imgcompute.h"
#include "savedata.h"
#include "compress.h"
#include "featurevector.h"
#include "SaveToPng.h"
#include "bpg.h"
#include "TwentyToOne.h"
#include "psnr.h"
#include "DataConvert.h"

//#include "NtoO.h"


#include "getopt.h"

#ifndef WIN32

#else
#include "getopt.h"
#endif

using namespace std;

#define USAGE_STRING "Usage: %s --input original_samples --output compressedImage"

struct option options[] = {
	{"input", 1, NULL, 1},
	{"output", 1, NULL, 2},
	{"ratio", 1, NULL, 3},
	{"start_dim", 1, NULL, 4},
	{"end_dim", 1, NULL, 5},
	{"img_height",1, NULL, 6},
	{"img_width",1, NULL, 7},
	{NULL, 0, NULL, 0}
};

int main(int argc, char* argv[])
{
	/**********************************************************************************************************************
	封装EXE代码
	**********************************************************/

	char samples_file[128];
	char out_file[128];
	int ratio = 1;
	int start_dim = 1;
	int end_dim = 224;
	int foundOpt = 0;
	int img_height = 680;
	int img_width = 512;

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
			ratio = (unsigned int)atoi(optarg);
		case 4:
			start_dim = (unsigned int)atoi(optarg);
		case 5:
			end_dim = (unsigned int)atoi(optarg);
		case 6:
			img_height = (unsigned int)atoi(optarg);
		case 7:
			img_width = (unsigned int)atoi(optarg);
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

	//char samples_file[]= "..\\data\\BSQSEnd.raw";
	//char out_file[] = "compress_out\\";//改成tempBpg,compress_out里放最后压缩好的一个文件

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
	strcat(workPath, dir);// workPath储存arg[0]的EXE所输入的工作路径-"D:\xx\xx\"

	clock_t total_start, total_stop, compress_start, compress_stop, bpg_start, bpg_stop; //clock_t为clock()函数返回的变量类型
	double total_duration, compress_duration, bpg_duration;

	// 总程序计时开始
	total_start = clock();


	/**********************************************************************************************************************
	初始化结构体/变量
	***********************************************************/
	RawImag MyRaw; // raw文件数据
	Image ImagMean;// 均值数据
	CovData ImageCov; // 协方差
	Imagdata imagdata; // 图像数据
	Matrix K; //变换矩阵

	double ps;
	int png_num = 1;//KLT生成png的数量

	//分块声明
	RawImag MyRaw1;
	RawImag MyRaw2;

	//Image ImagMean1;
	//Image ImagMean2;


	Imagdata imagdata1;
	Imagdata imagdata2;

	CovData ImageCov1;
	CovData ImageCov2;

	Matrix K1;
	Matrix K2;

	char saveKPath1[100];
	char saveMeanPath1[100];
	strcpy(saveKPath1, workPath);
	strcat(saveKPath1, "tempBpg\\K1.dat");//saveKPath 为输出目录下的K.dat

	strcpy(saveMeanPath1, workPath);
	strcat(saveMeanPath1, "tempBpg\\mean.dat");//saveMeanPath 为输出目录下的mean.dat

	char saveKPath2[100];
	//char saveMeanPath2[100];
	strcpy(saveKPath2, workPath);
	strcat(saveKPath2, "tempBpg\\K2.dat");//saveKPath 为输出目录下的K.dat
	
	//strcpy(saveMeanPath2, workPath);
	//strcat(saveMeanPath2, "tempBpg\\mean2.dat");//saveMeanPath 为输出目录下的mean.dat


	char savePngPath[100];
	strcpy(savePngPath, workPath);
	strcat(savePngPath, "tempKlt\\");

	char inputBPG[100];
	char outputBPG[100];
	strcpy(inputBPG, workPath);
	strcat(inputBPG, "tempKlt\\");
	strcpy(outputBPG, workPath);
	strcat(outputBPG, "tempBpg\\");

	char inputHebing[100];
	char outputHebing[100];
	strcpy(inputHebing, workPath);
	strcat(inputHebing, "tempBpg\\*");
	strcpy(outputHebing, out_file);


	/**********************************************************************************************************************
	读取源数据，数据预处理
	imagedata 像素数据减去维度均值所得
	ImagMean 均值矩阵
	ImageCov 协方差矩阵
	***********************************************************/
	Readimage(&MyRaw, samples_file, img_height, img_width, start_dim, end_dim);//读取数据, MyRaw是原始图像数据	// 0.10s

	/**********************************************************************************************************************
	数据归一化
	***********************************************************/
	imagecov(&MyRaw, &imagdata, &ImagMean); // 

	/*
	分块操作-0923
	*/

	imagdata1.Col = imagdata.Col;
	imagdata1.Row = imagdata.Row;
	imagdata1.imgSize = imagdata.imgSize;
	imagdata1.Dim = imagdata.Dim / 2;
	imagdata1.rawSize = imagdata.rawSize / 2;

	imagdata1.data = (cBYTE*)malloc(sizeof(cBYTE) * imagdata1.rawSize);
	for (int i = 0; i < imagdata1.rawSize; i++) {
		imagdata1.data[i] = imagdata.data[i];
	}

	imagdata2.Col = imagdata.Col;
	imagdata2.Row = imagdata.Row;
	imagdata2.imgSize = imagdata.imgSize;
	imagdata2.Dim = imagdata.Dim / 2;
	imagdata2.rawSize = imagdata.rawSize / 2;

	imagdata2.data = (cBYTE*)malloc(sizeof(cBYTE) * imagdata2.rawSize);
	for (int i = 0; i < imagdata2.rawSize; i++) {
		imagdata2.data[i] = imagdata.data[i + imagdata2.rawSize];
	}

#pragma omp parallel sections
	{
#pragma omp section
		{
		printf("线程1: %d \n", omp_get_thread_num());
		//imagecov(&MyRaw1, &imagdata1, &ImagMean1);
		imagecompute_gpu(&imagdata1, &ImageCov1);
		featVec(&ImageCov1, &K1);
		reduceDim(&K1, png_num);
		retimagedata(&imagdata1, &K1);
		DataConvert(&imagdata1);
		saveK(&K1, saveKPath1);
		//savemean(&ImagMean1, saveMeanPath1);
		savetopng1(&imagdata1, savePngPath);
		}
#pragma omp section
		{
		printf("线程2: %d \n", omp_get_thread_num());
		//imagecov(&MyRaw2, &imagdata2, &ImagMean2);
		imagecompute_gpu(&imagdata2, &ImageCov2);
		featVec(&ImageCov2, &K2);
		reduceDim(&K2, png_num);
		retimagedata(&imagdata2, &K2);
		DataConvert(&imagdata2);
		saveK(&K2, saveKPath2);
		//savemean(&ImagMean2, saveMeanPath2);
		savetopng2(&imagdata2, savePngPath);
		}
	}

	// 拼接所有结构体数据
	// 1.MyRaw 不用拼 要释放

	// 2.imagdata
	//imagdata.data = (cBYTE*)malloc(sizeof(cBYTE) * MyRaw.rawSize);
	//imagdata.Col = MyRaw.Col;
	//imagdata.Row = MyRaw.Row;
	//imagdata.Dim = MyRaw.Dim;
	//imagdata.rawSize = MyRaw.rawSize;
	//imagdata.imgSize = MyRaw.imgSize;
	//int fenge = imagdata.rawSize / 2;
	//for (int i = 0; i < fenge; i++) {
	//	imagdata.data[i] = imagdata1.data[i];
	//}
	//for (int i = 0; i < fenge; i++) {
	//	imagdata.data[i + fenge] = imagdata2.data[i];
	//}

	// 3.ImagMean
	//ImagMean.data = (rBYTE*)malloc(sizeof(rBYTE) * MyRaw.imgSize);
	//ImagMean.Col = MyRaw.Col;
	//ImagMean.Row = MyRaw.Row;

	// 4.ImageCov
	//ImageCov.data = new covBYTE[sizeof(covBYTE) * imagdata.Dim * imagdata.Dim];
	//ImageCov.row = imagdata.Dim;
	//ImageCov.column = imagdata.Dim;

	// 5.K
	//K.data = (VBYTE*)malloc(sizeof(VBYTE) * ImageCov.row * ImageCov.column);
	//K.column = ImageCov.column;
	//K.row = ImageCov.row;

	savemean(&ImagMean, saveMeanPath1);

	bpg(workPath, inputBPG, outputBPG, ratio, png_num);
	twentyToOne(workPath, inputHebing, outputHebing);

	//全部free
	free(MyRaw.data);
	MyRaw.data = NULL;

	free(imagdata.data);
	imagdata.data = NULL;
	free(imagdata1.data);
	imagdata1.data = NULL;
	free(imagdata2.data);
	imagdata2.data = NULL;

	//free(ImageCov.data);
	//ImageCov.data = NULL;
	free(ImageCov1.data);
	ImageCov1.data = NULL;
	free(ImageCov2.data);
	ImageCov2.data = NULL;

	free(ImagMean.data);
	ImagMean.data = NULL;

	//free(K.data);
	//K.data = NULL;
	free(K1.data);
	K1.data = NULL;
	free(K2.data);
	K2.data = NULL;


	// 总程序计时结束
	total_stop = clock();
	total_duration = (double)(total_stop - total_start ) / CLK_TCK; //CLK_TCK为clock()函数的时间单位
	printf("def main：整个程序. 时间：%fs\n", total_duration - 1);

	//测试函数-test.cpp
	//testfun1(&imagdata, &ImageCov);


	getchar();

	return 0;




	/**********************************************************************************************************************
	数据归一化
	***********************************************************/
	//imagecov(&MyRaw, &imagdata, &ImagMean); // 0.51s


	/*********************************************************************************************************************
	GPU 计算协方差
	***********************************************************/
	//imagecompute_gpu(&imagdata, &ImageCov); // 1.60s

	//int min_data;
	//int max_data;

	//int data_len = imagdata.rawSize;

	//min_data = *min_element(imagdata.data, imagdata.data + data_len); // -10447
	//max_data = *max_element(imagdata.data, imagdata.data + data_len); // 61880

	//printf("GPU后最小值:%d\n", min_data);
	//printf("GPU后最大值:%d\n", max_data);


	//原版本调用CPU
	//imagecompute(&imagdata, &ImageCov);

	//free(MyRaw.data); //原始图像数据不再使用，后续计算使用imagdata，释放内存。
	//MyRaw.data = NULL;


	/***********************************************************************************************************************
	计算特征向量/特征值
	***********************************************************/
	//featVec(&ImageCov, &K);//计算协方差矩阵的特征向量、特征值	// 1.44s


	/***********************************************************************************************************************
	生成降维矩阵
	***********************************************************/
	//reduceDim(&K, png_num);//变换矩阵降维	// 0.1s

	//data_len = imagdata.rawSize;

	//min_data = *min_element(imagdata.data, imagdata.data + data_len); // -10447
	//max_data = *max_element(imagdata.data, imagdata.data + data_len); // 61880

	//printf("降维前最小值:%d\n", min_data);
	//printf("降维前最大值:%d\n", max_data);


	/***********************************************************************************************************************
	通过降维矩阵压缩
	***********************************************************/
	//retimagedata(&imagdata, &K);//图像降维压缩 #imagedata的最小值为 -50734 最大值为81416  相加为132150	// 0.80s

	//int data_len = imagdata.rawSize;

	//int min_data = *min_element(imagdata.data, imagdata.data + data_len); // -10447 -5	// -3149 -1
	//int max_data = *max_element(imagdata.data, imagdata.data + data_len); // 61880  -5   // 61880 -1

	//printf("降维后最小值:%d\n", min_data);
	//printf("降维后最大值:%d\n", max_data);

	/***********************************************************************************************************************
	压缩数据格式处理，16位转8位
	***********************************************************/
	//DataConvert(&imagdata);	// 0.02s


	//保存图像为png文件
	//savetopng(&imagdata, "..\\temp\\");


	/***********************************************************************************************************************
	保存PNG
	***********************************************************/
	//char savePngPath[100];
	////PngPath与当前目录拼接
	//strcpy(savePngPath, workPath);
	//strcat(savePngPath, "tempKlt\\");
	//savetopng(&imagdata, savePngPath);	// 0.59s


	//保存变换矩阵
	//saveK(&K, "..\\out\\K.dat");
	//savemean(&ImagMean, "..\\out\\mean.dat");


	/***********************************************************************************************************************
	保存K变换矩阵，保存Mean均差
	***********************************************************/
	//char saveKPath[100];
	//char saveMeanPath[100];
	//strcpy(saveKPath, workPath);
	//strcat(saveKPath, "tempBpg\\K.dat");//saveKPath 为输出目录下的K.dat
	//strcpy(saveMeanPath, workPath);
	//strcat(saveMeanPath, "tempBpg\\mean.dat");//saveMeanPath 为输出目录下的mean.dat
	//saveK(&K, saveKPath);
	//savemean(&ImagMean, saveMeanPath);


	/***********************************************************************************************************************
	调用BPG
	***********************************************************/
	//char inputBPG[100];
	//char outputBPG[100];
	////strcpy(inputBPG, " ..\\temp\\");
	////strcpy(outputBPG, "..\\out\\");
	////命令行 在同一根目录下
	//strcpy(inputBPG, workPath);
	//strcat(inputBPG, "tempKlt\\");
	//strcpy(outputBPG, workPath);
	//strcat(outputBPG, "tempBpg\\");
	//bpg(workPath, inputBPG, outputBPG, ratio, png_num);	//(输入png路径，输出bpg路径) // 1.005s  0915-0.236s


	//利用RAR方式进行文件合并操作
	//char inputHebing[100];
	//char outputHebing[100];

	//strcpy(inputHebing, "..\\out\\*");
	//strcpy(outputHebing, "..\\..\\final.hyq");


	/***********************************************************************************************************************
	多文件rar压缩成单文件
	***********************************************************/
	//char inputHebing[100];
	//char outputHebing[100];
	//strcpy(inputHebing, workPath);
	//strcat(inputHebing, "tempBpg\\*");
	//strcpy(outputHebing, out_file);
	//twentyToOne(workPath, inputHebing, outputHebing); // 0.117s

	////全部free
	//free(MyRaw1.data);
	//MyRaw1.data = NULL;


	//// 总程序计时结束
	//total_stop = clock();
	//total_duration = (double)(total_stop - total_start) / CLK_TCK; //CLK_TCK为clock()函数的时间单位
	//printf("def main：整个程序. 时间：%fs\n", total_duration);

	////测试函数-test.cpp
	////testfun1(&imagdata, &ImageCov);


	//getchar();

	//return 0;

}

