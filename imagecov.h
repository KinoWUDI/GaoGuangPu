#ifndef IMAGECOV_H
#define IMAGECOV_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include<malloc.h>
#include"Readimage.h"

typedef int covBYTE;
typedef int cBYTE;

typedef struct {
	int Row;
	int Col;
	rBYTE* data;
}Image;

typedef struct //rawÍ¼Ïñ¼õ¾ùÖµ
{
	int Row;
	int Col;
	int Dim;
	int rawSize;
	int imgSize;
	cBYTE* data;
}Imagdata;

typedef struct
{
	int row;
	int column;
	covBYTE* data;
}CovData;

void imagecov(RawImag*, Imagdata*, Image*);

#ifdef __cplusplus
}
#endif

#endif