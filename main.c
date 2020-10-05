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
	��װEXE����
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
	strcat(workPath, dir);// workPath����arg[0]������Ĺ���·��-"D:\xx\xx\"


	Image imagemean;//��ֵ
	//Image imagemean1;//��ֵ1
	//Image imagemean2;//��ֵ2


	Matrix K;//�任����
	Matrix K1;//�任����1
	Matrix K2;//�任����2


	Imagdata imagdata;//��һ��ͼ������
	Imagdata imagdata1;//��һ��ͼ������1
	Imagdata imagdata2;//��һ��ͼ������2



	RawImag resRaw;//raw�ļ�����

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

	//1��ѹ���ļ���ѹ��22���ļ�
	//rBPG();

	//4.26��ʱ����RAR��ʽ�����ļ���ֲ���
	//char inputChaifen[100];
	//char outputChaifen[100];

	//strcpy(inputChaifen, "..\\..\\final.hyq");
	//strcpy(outputChaifen, "..\\in");

	//�����е���
	char inputChaifen[100];
	char outputChaifen[100];

	strcpy(inputChaifen, samples_file);//����hyqѹ���ļ�·��

	strcpy(outputChaifen, workPath);
	strcat(outputChaifen, "tempRBPG\\");//���BPG,K,MEAN�ļ�·��

	oneToTwenty(workPath, inputChaifen, outputChaifen);

	printf("BPGת��ΪPNG���̿�ʼ...\n");

	//---------------------------------------
	//��ȡbpgתpng

	char num[4];

	for (int i = 0; i < 2; ++i) {
		//���������ַ���
		//char comSta[100] = "..\\..\\bpg\\bpgdec -o temp\\";

		char rbpgPath[500];
		strcpy(rbpgPath, workPath);

		//������
		char comSta[100] = "bpg\\bpgdec -o ";
		strcat(rbpgPath, comSta);

		char tempPngPath[100];
		strcpy(tempPngPath, workPath);
		strcat(tempPngPath, "temp\\");

		strcat(rbpgPath, tempPngPath);

		_itoa(i, num, 10);
		strcat(rbpgPath, num);//���������
		//strcat(comSta, "r.png  ..\\..\\KLT\\out\\");//�����ļ�·��	4.25��ǰ�汾
		strcat(rbpgPath, "r.png ");//�����ļ�·��+ȫ��

		char tempRBpgPath[100];
		strcpy(tempRBpgPath, workPath);
		strcat(tempRBpgPath, "tempRBPG\\");

		strcat(rbpgPath, tempRBpgPath);

		strcat(rbpgPath, num);//��ȡ�ļ������
		strcat(rbpgPath, "bpg.bpg");//��ȡ�ļ���ȫ��
		//ִ������
		system(rbpgPath);
		//system("..\\..\\bpg\\bpgdec -o ..\\temp\\18r.png  ..\\..\\KLT\\out\\18bpg.bpg");
	}

	printf("BPGת��ΪPNG�������\n");
	//---------------------------------------
	/*����PNG*/
	//---------------------------------------

	printf("����PNG���̿�ʼ...\n");
	//��ȡ
	//system("..\\p2b.exe");

	//���ĵ�ǰ����·�����������atg[0]����
	char buf[1000];
	strcpy(buf, workPath);

	SetCurrentDirectory(buf);  //���ù���·��

	//GetCurrentDirectory(1000, buf);

	//������
	char p2bPath[100];
	strcpy(p2bPath, workPath);
	strcat(p2bPath, "p2b.exe");

	system(p2bPath);
	/*ֻ�ܶ�ȡ��ǰĿ¼�µ�temp�ļ������ļ���Ϊ��0r.png�������ļ���temp�ļ��в��ܷ��������κ��ļ�
	���������ļ��ӵ���ǰĿ¼�ļ��С�temp0����*/
	unsigned char* tempdata8= (unsigned char*)malloc(sizeof(unsigned char)* 680*512*2);//���ڶ�ջ������ռ䣬�ڶ�������

	for (int i = 0; i < 2; i++) {
		_itoa(i, num, 10);

		char outname[100];
		strcpy(outname, workPath);

		//������
		char comSta[20] = "temp0\\rdata";
		strcat(outname, comSta);

		strcat(outname, num);
		strcat(outname, ".dat");
		readChardat((tempdata8+i*680*512),outname, 680 * 512);
	}

	//CBYTE* tempdataB = (CBYTE*)malloc(sizeof(CBYTE) * imagdata.rawSize);
	//for (int i = 0; i < imagdata.rawSize / 2; i++) {
	//	//tempdataB[i] = (tempdata8[i]/256.0)*72327 - 10447; // 5��PNG
	//	tempdataB[i] = (tempdata8[i] / 256.0) * 65029 - 3149;// 1��PNG
	//}

	/*
	������
	*/

	CBYTE* tempdataB = (CBYTE*)malloc(sizeof(CBYTE) * imagdata.rawSize);
	for (int i = 0; i < imagdata.rawSize / 2; i++) {
		//tempdataB[i] = (tempdata8[i]/256.0)*72327 - 10447; // 5��PNG
		tempdataB[i] = (tempdata8[i] / 256.0) * 36991 - 457;// 1��PNG
	}

	for (int i = imagdata.rawSize / 2; i < imagdata.rawSize; i++) {
		//tempdataB[i] = (tempdata8[i]/256.0)*72327 - 10447; // 5��PNG
		tempdataB[i] = (tempdata8[i] / 256.0) * 44164 - 173;// 1��PNG
	}


	imagdata.data = tempdataB;

	//imagdata.data = tempdataB;
	free(tempdata8);

	printf("����PNG�������\n");

	//readK(&K,"..\\..\\KLT\\out\\K.dat");//  4.25��ǰ�汾
	//readmean(&imagemean, "..\\..\\KLT\\out\\mean.dat");//	4.25��ǰ�汾

	//readK(&K, "..\\in\\K.dat");
	//readmean(&imagemean, "..\\in\\mean.dat");

	//������
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

	printf("��ȡK��MEAN�ļ��ɹ�\n");

	reimage(&imagemean, &K, &imagdata,&resRaw);//�ع�ͼ��
	//imagre(&resRaw, "out\\restore.raw");//����Ϊraw�ļ�

	//������
	imagre(&resRaw, out_file);//����Ϊraw�ļ�

	getchar();

	return 0;

}