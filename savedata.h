#ifndef SAVEDATA_H
#define SAVEDATA_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include"Readimage.h"
#include"imagecov.h"
#include"featurevector.h"
#include<stdbool.h>
#include<stdio.h>

	bool saveCovBYTEtofile(covBYTE*, char*, int);
	covBYTE* readCovBYTEfromfile(char*, int);

	bool imagout(Image*, Matrix*, Imagdata*, char*);
	bool imagoutjustpic(Imagdata*, char*);
	bool savemean(Image* imagdata, const char filename[]);
	bool saveK(Matrix* K, const char filename[]);


#ifdef __cplusplus
}
#endif

#endif