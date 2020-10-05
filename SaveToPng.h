#ifndef SAVETOPNG_H
#define SAVETOPNG_H


#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include"Readimage.h"
#include"imagecov.h"
#include "svpng.inc"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

	void savetopng1(Imagdata*, char* savepath);
	void savetopng2(Imagdata*, char* savepath);

#ifdef __cplusplus
}
#endif

#endif