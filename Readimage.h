#pragma once
#ifndef READIMAGE_H
#define READIMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include<stdio.h>
#include<malloc.h>
#include<stdbool.h>

	typedef unsigned short rBYTE;
	typedef struct //rawÍ¼Ïñ½á¹¹Ìå
	{
		int Row;
		int Col;
		int Dim;
		int rawSize;
		int imgSize;
		rBYTE* data;
	}RawImag;
	bool Readimage(RawImag* raw, char* path, int row, int col, int start_dim, int end_dim);

#ifdef __cplusplus
}
#endif

#endif
