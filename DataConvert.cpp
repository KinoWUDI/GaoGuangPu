#include "DataConvert.h"
#include <Windows.h>
#include <time.h>
#include <algorithm>
#include <omp.h>

using namespace std;

void DataConvert(Imagdata* imagdata) {

	// ��ʱ��ʼ
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();

	int min_data;
	int max_data;

	int data_len = imagdata->rawSize;

	/*
	�����飬��һ��max:36534	min:-457���ڶ���max:43991 min:173.
	*/

	min_data = *min_element(imagdata->data, imagdata->data + data_len); // -10447  
	max_data = *max_element(imagdata->data, imagdata->data + data_len); // 61880		

	printf("������Сֵ:%d\n",min_data);
	printf("�������ֵ:%d\n",max_data);

	printf("�����߳�:%d\n", omp_get_thread_num());



	//�������ݸ�ʽ
	for (int i = 0; i < imagdata->rawSize; ++i) {
		imagdata->data[i] += abs(min_data);
		imagdata->data[i] = imagdata->data[i] / double(max_data + abs(min_data)) * 256; //
	};

	// ��ʱ����
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def DataConvert������ת��. ʱ�䣺%fs\n", duration);

}