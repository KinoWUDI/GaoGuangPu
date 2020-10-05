#ifndef COMPRESS_H
#define COMPRESS_H


#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include"Readimage.h"
#include"imagecov.h"
#include"featurevector.h"

	void reduceDim(Matrix*, int);
	void retimagedata(Imagdata* imagdata, Matrix* K);

#ifdef __cplusplus
}
#endif

#endif