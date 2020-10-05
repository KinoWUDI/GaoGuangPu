#include "DataConvert.h"
#include <Windows.h>
#include <time.h>
#include <algorithm>
#include <omp.h>

using namespace std;

void DataConvert(Imagdata* imagdata) {

	// 计时开始
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();

	int min_data;
	int max_data;

	int data_len = imagdata->rawSize;

	/*
	分两块，第一块max:36534	min:-457、第二快max:43991 min:173.
	*/

	min_data = *min_element(imagdata->data, imagdata->data + data_len); // -10447  
	max_data = *max_element(imagdata->data, imagdata->data + data_len); // 61880		

	printf("最终最小值:%d\n",min_data);
	printf("最终最大值:%d\n",max_data);

	printf("这是线程:%d\n", omp_get_thread_num());



	//调整数据格式
	for (int i = 0; i < imagdata->rawSize; ++i) {
		imagdata->data[i] += abs(min_data);
		imagdata->data[i] = imagdata->data[i] / double(max_data + abs(min_data)) * 256; //
	};

	// 计时结束
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def DataConvert：数据转换. 时间：%fs\n", duration);

}