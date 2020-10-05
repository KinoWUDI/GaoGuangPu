#include "psnr.h"
#include "Windows.h"
#include "stdio.h"
#include "math.h"

double caculate(rBYTE* image1, rBYTE* image2) {
    double mse = 0.0;
    double temp = 0.0;
    for (int i = 0; i < 512 * 680 * 224; i++)
    {
        //temp = temp + (image1[i] - image2[i])*(image1[i] - image2[i]);
        temp = temp + pow(image1[i] - image2[i], 2);
    }

    printf("temp=%lf\n", temp);
    mse = (double)(temp / (512 * 680 * 224)); //¾ù·½²î
    double t;
    t = (double)(65535.0 * 65535.0 / mse);  //(2^(n)-1)^2/mse //65535.0*65535.0
    double psnr;
    psnr = 10 * log10(t);
    printf("psnr=%lf\n", psnr);
    return psnr;
}
