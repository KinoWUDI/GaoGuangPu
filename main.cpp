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
	��װEXE����
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
			//����
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
	//char out_file[] = "compress_out\\";//�ĳ�tempBpg,compress_out������ѹ���õ�һ���ļ�

	//��ȡ��ǰEXE������·��
	//argv[0]�ǵ�ǰ���г����ȫ·����
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char workPath[100];

	_splitpath(argv[0], drive, dir, fname, ext);//drive:������	dir:·�� fname:�ļ��� ext:��׺
	strcpy(workPath, drive);
	strcat(workPath, dir);// workPath����arg[0]��EXE������Ĺ���·��-"D:\xx\xx\"

	clock_t total_start, total_stop, compress_start, compress_stop, bpg_start, bpg_stop; //clock_tΪclock()�������صı�������
	double total_duration, compress_duration, bpg_duration;

	// �ܳ����ʱ��ʼ
	total_start = clock();


	/**********************************************************************************************************************
	��ʼ���ṹ��/����
	***********************************************************/
	RawImag MyRaw; // raw�ļ�����
	Image ImagMean;// ��ֵ����
	CovData ImageCov; // Э����
	Imagdata imagdata; // ͼ������
	Matrix K; //�任����

	double ps;
	int png_num = 1;//KLT����png������

	//�ֿ�����
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
	strcat(saveKPath1, "tempBpg\\K1.dat");//saveKPath Ϊ���Ŀ¼�µ�K.dat

	strcpy(saveMeanPath1, workPath);
	strcat(saveMeanPath1, "tempBpg\\mean.dat");//saveMeanPath Ϊ���Ŀ¼�µ�mean.dat

	char saveKPath2[100];
	//char saveMeanPath2[100];
	strcpy(saveKPath2, workPath);
	strcat(saveKPath2, "tempBpg\\K2.dat");//saveKPath Ϊ���Ŀ¼�µ�K.dat
	
	//strcpy(saveMeanPath2, workPath);
	//strcat(saveMeanPath2, "tempBpg\\mean2.dat");//saveMeanPath Ϊ���Ŀ¼�µ�mean.dat


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
	��ȡԴ���ݣ�����Ԥ����
	imagedata �������ݼ�ȥά�Ⱦ�ֵ����
	ImagMean ��ֵ����
	ImageCov Э�������
	***********************************************************/
	Readimage(&MyRaw, samples_file, img_height, img_width, start_dim, end_dim);//��ȡ����, MyRaw��ԭʼͼ������	// 0.10s

	/**********************************************************************************************************************
	���ݹ�һ��
	***********************************************************/
	imagecov(&MyRaw, &imagdata, &ImagMean); // 

	/*
	�ֿ����-0923
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
		printf("�߳�1: %d \n", omp_get_thread_num());
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
		printf("�߳�2: %d \n", omp_get_thread_num());
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

	// ƴ�����нṹ������
	// 1.MyRaw ����ƴ Ҫ�ͷ�

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

	//ȫ��free
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


	// �ܳ����ʱ����
	total_stop = clock();
	total_duration = (double)(total_stop - total_start ) / CLK_TCK; //CLK_TCKΪclock()������ʱ�䵥λ
	printf("def main����������. ʱ�䣺%fs\n", total_duration - 1);

	//���Ժ���-test.cpp
	//testfun1(&imagdata, &ImageCov);


	getchar();

	return 0;




	/**********************************************************************************************************************
	���ݹ�һ��
	***********************************************************/
	//imagecov(&MyRaw, &imagdata, &ImagMean); // 0.51s


	/*********************************************************************************************************************
	GPU ����Э����
	***********************************************************/
	//imagecompute_gpu(&imagdata, &ImageCov); // 1.60s

	//int min_data;
	//int max_data;

	//int data_len = imagdata.rawSize;

	//min_data = *min_element(imagdata.data, imagdata.data + data_len); // -10447
	//max_data = *max_element(imagdata.data, imagdata.data + data_len); // 61880

	//printf("GPU����Сֵ:%d\n", min_data);
	//printf("GPU�����ֵ:%d\n", max_data);


	//ԭ�汾����CPU
	//imagecompute(&imagdata, &ImageCov);

	//free(MyRaw.data); //ԭʼͼ�����ݲ���ʹ�ã���������ʹ��imagdata���ͷ��ڴ档
	//MyRaw.data = NULL;


	/***********************************************************************************************************************
	������������/����ֵ
	***********************************************************/
	//featVec(&ImageCov, &K);//����Э����������������������ֵ	// 1.44s


	/***********************************************************************************************************************
	���ɽ�ά����
	***********************************************************/
	//reduceDim(&K, png_num);//�任����ά	// 0.1s

	//data_len = imagdata.rawSize;

	//min_data = *min_element(imagdata.data, imagdata.data + data_len); // -10447
	//max_data = *max_element(imagdata.data, imagdata.data + data_len); // 61880

	//printf("��άǰ��Сֵ:%d\n", min_data);
	//printf("��άǰ���ֵ:%d\n", max_data);


	/***********************************************************************************************************************
	ͨ����ά����ѹ��
	***********************************************************/
	//retimagedata(&imagdata, &K);//ͼ��άѹ�� #imagedata����СֵΪ -50734 ���ֵΪ81416  ���Ϊ132150	// 0.80s

	//int data_len = imagdata.rawSize;

	//int min_data = *min_element(imagdata.data, imagdata.data + data_len); // -10447 -5	// -3149 -1
	//int max_data = *max_element(imagdata.data, imagdata.data + data_len); // 61880  -5   // 61880 -1

	//printf("��ά����Сֵ:%d\n", min_data);
	//printf("��ά�����ֵ:%d\n", max_data);

	/***********************************************************************************************************************
	ѹ�����ݸ�ʽ����16λת8λ
	***********************************************************/
	//DataConvert(&imagdata);	// 0.02s


	//����ͼ��Ϊpng�ļ�
	//savetopng(&imagdata, "..\\temp\\");


	/***********************************************************************************************************************
	����PNG
	***********************************************************/
	//char savePngPath[100];
	////PngPath�뵱ǰĿ¼ƴ��
	//strcpy(savePngPath, workPath);
	//strcat(savePngPath, "tempKlt\\");
	//savetopng(&imagdata, savePngPath);	// 0.59s


	//����任����
	//saveK(&K, "..\\out\\K.dat");
	//savemean(&ImagMean, "..\\out\\mean.dat");


	/***********************************************************************************************************************
	����K�任���󣬱���Mean����
	***********************************************************/
	//char saveKPath[100];
	//char saveMeanPath[100];
	//strcpy(saveKPath, workPath);
	//strcat(saveKPath, "tempBpg\\K.dat");//saveKPath Ϊ���Ŀ¼�µ�K.dat
	//strcpy(saveMeanPath, workPath);
	//strcat(saveMeanPath, "tempBpg\\mean.dat");//saveMeanPath Ϊ���Ŀ¼�µ�mean.dat
	//saveK(&K, saveKPath);
	//savemean(&ImagMean, saveMeanPath);


	/***********************************************************************************************************************
	����BPG
	***********************************************************/
	//char inputBPG[100];
	//char outputBPG[100];
	////strcpy(inputBPG, " ..\\temp\\");
	////strcpy(outputBPG, "..\\out\\");
	////������ ��ͬһ��Ŀ¼��
	//strcpy(inputBPG, workPath);
	//strcat(inputBPG, "tempKlt\\");
	//strcpy(outputBPG, workPath);
	//strcat(outputBPG, "tempBpg\\");
	//bpg(workPath, inputBPG, outputBPG, ratio, png_num);	//(����png·�������bpg·��) // 1.005s  0915-0.236s


	//����RAR��ʽ�����ļ��ϲ�����
	//char inputHebing[100];
	//char outputHebing[100];

	//strcpy(inputHebing, "..\\out\\*");
	//strcpy(outputHebing, "..\\..\\final.hyq");


	/***********************************************************************************************************************
	���ļ�rarѹ���ɵ��ļ�
	***********************************************************/
	//char inputHebing[100];
	//char outputHebing[100];
	//strcpy(inputHebing, workPath);
	//strcat(inputHebing, "tempBpg\\*");
	//strcpy(outputHebing, out_file);
	//twentyToOne(workPath, inputHebing, outputHebing); // 0.117s

	////ȫ��free
	//free(MyRaw1.data);
	//MyRaw1.data = NULL;


	//// �ܳ����ʱ����
	//total_stop = clock();
	//total_duration = (double)(total_stop - total_start) / CLK_TCK; //CLK_TCKΪclock()������ʱ�䵥λ
	//printf("def main����������. ʱ�䣺%fs\n", total_duration);

	////���Ժ���-test.cpp
	////testfun1(&imagdata, &ImageCov);


	//getchar();

	//return 0;

}

