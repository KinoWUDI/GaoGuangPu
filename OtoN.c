#include"OtoN.h"


int rBPG() {
	FILE* fp = fopen("..\\..\\out.hyp", "rb");//�˴�д���1��ѹ���ļ�λ��
	int n;
	//char *bfs[500];
	int fillsizes[500];
	fread(&n, sizeof(int), 1, fp);
	//printf("ͼ�����%d", n);

	for (int i = 0; i < n; i++) {
		fread(&fillsizes[i], sizeof(int), 1, fp);
		//printf("�ļ���С%d\n", fillsizes[i]);
	}
	char* bfs;
	FILE* fp1;

	bfs = (char*)malloc(sizeof(char) * fillsizes[0]);
	fread(bfs, sizeof(char), fillsizes[0], fp);
	fp1 = fopen("..\\..\\out\\mean.dat", "wb");
	fwrite(bfs, sizeof(char), fillsizes[0], fp1);
	fclose(fp1);
	free(bfs);

	bfs = (char*)malloc(sizeof(char) * fillsizes[1]);
	fread(bfs, sizeof(char), fillsizes[1], fp);
	fp1 = fopen("..\\..\\out\\K.dat", "wb");
	fwrite(bfs, sizeof(char), fillsizes[1], fp1);
	fclose(fp1);
	free(bfs);



	for (int i = 0; i < n - 2; i++) {
		//���������д��ѭ���ڣ�ÿ��ѭ����comSta��num���á�
		char comSta[100] = "..\\..\\out\\"; //�˴���д�ؽ�n��.bpg�ļ���K.dat�ļ���mean.dat�ļ���Ŀ¼
		char num[4];
		bfs = (char*)malloc(sizeof(char) * fillsizes[i + 2]);
		fread(bfs, sizeof(char), fillsizes[i + 2], fp);
		itoa(i, num, 10);
		strcat(comSta, num);//���������
		strcat(comSta, "bpg.bpg");

		fp1 = fopen(comSta, "wb");
		fwrite(bfs, sizeof(char), fillsizes[i + 2], fp1);
		fclose(fp1);
		free(bfs);
	}

	fclose(fp);
	return 0;
}
