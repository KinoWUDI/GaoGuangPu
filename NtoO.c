#include"NtoO.h"
/*
��filename1��filename2��comSta��λ��KLT���Ŀ¼������n��.bpg�ļ���K.dat�ļ���mean.dat�ļ�

77���޸�Ϊ1��ѹ���ļ����λ��
*/
int getfillsize(FILE* f) {//��ȡ�ļ���С����λ���ֽ� 
	FILE* fp = f;
	FILE* fq = f;
	int len;
	fpos_t posthead;
	fpos_t postend;
	fseek(fp, 0, SEEK_SET);//���ļ���ָ��fp�ض�λ�����ļ�ָ�룬ƫ������λ�ò�����λ�ò�����SEEK_CUR,SEEK_END,SEEK_SET��
	fgetpos(fp, &posthead);//����int
	fseek(fq, 0, SEEK_END);
	fgetpos(fq, &postend);
	len = (int)(postend - posthead);
	return len;
}

int changeBpg(int n) {
	int fillsizes[500];//����ÿ���ļ��Ĵ�С���ļ���������
	char* bfs[500];//����ÿ���ļ���ָ�룬�ļ������̶�
	//�ȶ�ȡ��ֵ��ֵ�ͱ任����
	char filename1[] = "..\\out\\mean.dat";//�����ļ���
	char filename2[] = "..\\out\\K.dat";
	FILE* fp = fopen(filename1, "rb");//��ȡ�ļ�
	if (fp == NULL) {
		printf("��ȡ�ļ�ʧ��/n");
		return 0;
	}
	fillsizes[0] = getfillsize(fp);//��ȡ�ļ���С����λ���ֽ�
	bfs[0] = (char*)malloc(sizeof(char) * fillsizes[0]);//����ռ�       �ļ���С������s:ʹ��realloc��չԭʼ�ռ�
	//cBYTE* datatemp = new cBYTE[sizeof(cBYTE) * imagdata->rawSize]; // rawSize ��512x680x224,8000W
	fread(bfs[0], sizeof(char), fillsizes[0], fp);
	//for (int i = 0; i < fillsizes[0]; ++i) {
	//	fread(&bfs[0], sizeof(char), 1, fp);
	//}
	fclose(fp);

	fp = fopen(filename2, "rb");//��ȡ�ļ�
	if (fp == NULL) {
		printf("��ȡ�ļ�ʧ��/n");
		return 0;
	}
	fillsizes[1] = getfillsize(fp);
	bfs[1] = (char*)malloc(sizeof(char) * fillsizes[1] );//����ռ�       �ļ���С������s:ʹ��realloc��չԭʼ�ռ�
	fread(bfs[1], sizeof(char), fillsizes[1], fp);
	//for (int i = 0; i < fillsizes[1]; ++i) {
	//	fread(&bfs[1], sizeof(char), 1, fp);
	//}
	fclose(fp);

	//printf("%f", fillsizes[0]);

	for (int i = 2; i < n + 2; i++) {
		char comSta[100] = "..\\out\\";//�ļ�Ŀ¼
		char num[4];
		itoa(i - 2, num, 10);
		strcat(comSta, num);
		strcat(comSta, "bpg.bpg");
		//char filename = 'a';//�����ļ���
		fp = fopen(comSta, "rb");//��ȡ�ļ�
		if (fp == NULL) {
			printf("��ȡ�ļ�%dʧ��/n", i);
			continue;
		}
		printf("��ȡ�ļ�%s ", comSta);

		fillsizes[i] = getfillsize(fp);//��ȡ�ļ���С��
		printf("�ļ���С%d\n", fillsizes[i]);

		bfs[i] = (char*)malloc(sizeof(char) * fillsizes[i] );//����ռ�       �ļ���С������s:ʹ��realloc��չԭʼ�ռ�
		fread(bfs[i], sizeof(char), fillsizes[0], fp);
		//for (int i = 0; i < fillsizes[0]; ++i) {
		//	fread(&bfs[i], sizeof(char), 1, fp);
		//}
		fclose(fp);
	}
	//����Ϊд��һ���ļ�
	FILE* fp1 = fopen("..\\..\\out.hyp", "wb");//�˴�д��󱣴�1��ѹ���ļ�λ��
	if (fp1 == NULL) {
		printf("�����ļ�ʧ��/n");
		return false;
	}
	int b = n + 2;
	fwrite(&b, sizeof(int), 1, fp1);

	for (int i = 0; i < b; i++) {
		fwrite(&fillsizes[i], sizeof(int), 1, fp1);
	}

	for (int i = 0; i < b; i++) {
		fwrite(bfs[i], sizeof(char), fillsizes[i], fp1);
		//printf("�����ļ�%d\n", i);
	}

	fclose(fp1);
	printf("ѹ��ͼ�񱣴�ɹ�\n");
	return 0;
}

