#include"OtoN.h"


int rBPG() {
	FILE* fp = fopen("..\\..\\out.hyp", "rb");//此处写最后1个压缩文件位置
	int n;
	//char *bfs[500];
	int fillsizes[500];
	fread(&n, sizeof(int), 1, fp);
	//printf("图像个数%d", n);

	for (int i = 0; i < n; i++) {
		fread(&fillsizes[i], sizeof(int), 1, fp);
		//printf("文件大小%d\n", fillsizes[i]);
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
		//下两句必须写在循环内，每次循环对comSta和num重置。
		char comSta[100] = "..\\..\\out\\"; //此处填写重建n个.bpg文件，K.dat文件和mean.dat文件的目录
		char num[4];
		bfs = (char*)malloc(sizeof(char) * fillsizes[i + 2]);
		fread(bfs, sizeof(char), fillsizes[i + 2], fp);
		itoa(i, num, 10);
		strcat(comSta, num);//保存名序号
		strcat(comSta, "bpg.bpg");

		fp1 = fopen(comSta, "wb");
		fwrite(bfs, sizeof(char), fillsizes[i + 2], fp1);
		fclose(fp1);
		free(bfs);
	}

	fclose(fp);
	return 0;
}
