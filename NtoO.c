#include"NtoO.h"
/*
将filename1，filename2，comSta定位到KLT输出目录，包括n个.bpg文件，K.dat文件和mean.dat文件

77行修改为1个压缩文件输出位置
*/
int getfillsize(FILE* f) {//获取文件大小；单位是字节 
	FILE* fp = f;
	FILE* fq = f;
	int len;
	fpos_t posthead;
	fpos_t postend;
	fseek(fp, 0, SEEK_SET);//将文件流指针fp重定位，（文件指针，偏移量，位置参数）位置参数：SEEK_CUR,SEEK_END,SEEK_SET。
	fgetpos(fp, &posthead);//返回int
	fseek(fq, 0, SEEK_END);
	fgetpos(fq, &postend);
	len = (int)(postend - posthead);
	return len;
}

int changeBpg(int n) {
	int fillsizes[500];//储存每个文件的大小，文件个数不定
	char* bfs[500];//储存每个文件的指针，文件数不固定
	//先读取均值均值和变换矩阵
	char filename1[] = "..\\out\\mean.dat";//构建文件名
	char filename2[] = "..\\out\\K.dat";
	FILE* fp = fopen(filename1, "rb");//读取文件
	if (fp == NULL) {
		printf("读取文件失败/n");
		return 0;
	}
	fillsizes[0] = getfillsize(fp);//获取文件大小；单位是字节
	bfs[0] = (char*)malloc(sizeof(char) * fillsizes[0]);//申请空间       文件大小不定，s:使用realloc扩展原始空间
	//cBYTE* datatemp = new cBYTE[sizeof(cBYTE) * imagdata->rawSize]; // rawSize 是512x680x224,8000W
	fread(bfs[0], sizeof(char), fillsizes[0], fp);
	//for (int i = 0; i < fillsizes[0]; ++i) {
	//	fread(&bfs[0], sizeof(char), 1, fp);
	//}
	fclose(fp);

	fp = fopen(filename2, "rb");//读取文件
	if (fp == NULL) {
		printf("读取文件失败/n");
		return 0;
	}
	fillsizes[1] = getfillsize(fp);
	bfs[1] = (char*)malloc(sizeof(char) * fillsizes[1] );//申请空间       文件大小不定，s:使用realloc扩展原始空间
	fread(bfs[1], sizeof(char), fillsizes[1], fp);
	//for (int i = 0; i < fillsizes[1]; ++i) {
	//	fread(&bfs[1], sizeof(char), 1, fp);
	//}
	fclose(fp);

	//printf("%f", fillsizes[0]);

	for (int i = 2; i < n + 2; i++) {
		char comSta[100] = "..\\out\\";//文件目录
		char num[4];
		itoa(i - 2, num, 10);
		strcat(comSta, num);
		strcat(comSta, "bpg.bpg");
		//char filename = 'a';//构建文件名
		fp = fopen(comSta, "rb");//读取文件
		if (fp == NULL) {
			printf("读取文件%d失败/n", i);
			continue;
		}
		printf("读取文件%s ", comSta);

		fillsizes[i] = getfillsize(fp);//获取文件大小；
		printf("文件大小%d\n", fillsizes[i]);

		bfs[i] = (char*)malloc(sizeof(char) * fillsizes[i] );//申请空间       文件大小不定，s:使用realloc扩展原始空间
		fread(bfs[i], sizeof(char), fillsizes[0], fp);
		//for (int i = 0; i < fillsizes[0]; ++i) {
		//	fread(&bfs[i], sizeof(char), 1, fp);
		//}
		fclose(fp);
	}
	//以下为写成一个文件
	FILE* fp1 = fopen("..\\..\\out.hyp", "wb");//此处写最后保存1个压缩文件位置
	if (fp1 == NULL) {
		printf("保存文件失败/n");
		return false;
	}
	int b = n + 2;
	fwrite(&b, sizeof(int), 1, fp1);

	for (int i = 0; i < b; i++) {
		fwrite(&fillsizes[i], sizeof(int), 1, fp1);
	}

	for (int i = 0; i < b; i++) {
		fwrite(bfs[i], sizeof(char), fillsizes[i], fp1);
		//printf("保存文件%d\n", i);
	}

	fclose(fp1);
	printf("压缩图像保存成功\n");
	return 0;
}

