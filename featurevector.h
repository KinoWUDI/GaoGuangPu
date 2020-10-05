#ifndef FEATUREVECTOR_H
#define FEATUREVECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include"Readimage.h"
#include "imagecov.h"
#include "imgcompute.h"
#include<math.h>
#include<stdbool.h>

typedef float VBYTE;
typedef struct //æÿ’ÛΩ·ππÃÂ
{
	int row;
	int column;
	VBYTE* data;
}Matrix;

void featVec(CovData*, Matrix*);

#ifdef __cplusplus
}
#endif

#endif