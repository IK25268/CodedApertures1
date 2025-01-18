#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Program.h"
#include "Matrix.h"
#include "Complex.h"
#include "FFT2D.h"
#include "LinFLTR.h"
#include "RWImg.h"

void Start()
{
    Matrix* matrixSRC = ReadImg("det31convert.bmp");
    Matrix* matrixAPT = ReadImg("aperture.bmp");

    FFTLaunchMatrix(matrixSRC);
    FFTLaunchMatrix(matrixAPT);
    MultiplFFTTgtSrc(matrixSRC, matrixAPT);
    RevFFTLaunchMatrix(matrixSRC);
    NormalMatrix(matrixSRC);
    PrintMatrix(matrixSRC);
    SaveImg(matrixSRC, "det31convertEDIT.bmp");
    SaveImg(matrixAPT, "apertureEDIT.bmp");


    // for (unsigned int count = 0; count < 10*2; count++)
    // {
    //     LinArr* testFFTArr1 = CreateLinArr(1024);
    //     LinArr* testFFTArr2 = CreateLinArr(1024);
    //     // LinArr* testFFTArr3 = CreateLinArr(8);
    //     // for (unsigned int x = 0; x < 4; x++)
    //     // {
    //     //     Compl buf1 = {rand()%5, 0.0};
    //     //     Compl buf2 = {rand()%5, 0.0};
    //     //     Compl buf3 = {0.0, 0.0};
    //     //     BstInsert(testFFTArr1->arr, NULL, &buf1, &x);
    //     //     BstInsert(testFFTArr2->arr, NULL, &buf2, &x);
    //     //     BstInsert(testFFTArr3->arr, NULL, &buf3, &x);
    //     // }
    //     // for (unsigned int x = 4; x < 8; x++)
    //     // {
    //     //     Compl buf1 = {0.0, 0.0};
    //     //     Compl buf2 = {0.0, 0.0};
    //     //     Compl buf3 = {0.0, 0.0};
    //     //     BstInsert(testFFTArr1->arr, NULL, &buf1, &x);
    //     //     BstInsert(testFFTArr2->arr, NULL, &buf2, &x);
    //     //     BstInsert(testFFTArr3->arr, NULL, &buf3, &x);
    //     // }
    //     for (unsigned int x = 0; x < 1024; x++)
    //     {
    //         Compl buf1 = {rand()%5, 0.0};
    //         Compl buf2 = {rand()%5, 0.0};
    //         BstInsert(testFFTArr1->arr, NULL, &buf1, &x);
    //         BstInsert(testFFTArr2->arr, NULL, &buf2, &x);
    //     }
    //     // PrintLinArr(testFFTArr1);
    //     // PrintLinArr(testFFTArr2);
    //     // ConvLin(testFFTArr3, testFFTArr1, testFFTArr2);
    //     // printf("print 1 2 Conv: "); 
    //     // PrintLinArr(testFFTArr3);
    //     FFTLin(testFFTArr1);
    //     FFTLin(testFFTArr2);
    //     // printf("print 1 FFT: "); 
    //     // PrintLinArr(testFFTArr1);
    //     // printf("print 2 FFT: "); 
    //     // PrintLinArr(testFFTArr2);
    //     MultFreqLin(testFFTArr1, testFFTArr2);
    //     // printf("print 1 2 FFT Mult: "); 
    //     // PrintLinArr(testFFTArr1); 
    //     RevFFTLin(testFFTArr1); 
    //     // printf("print 1 2 RevFFT Mult: "); 
    //     // PrintLinArr(testFFTArr1);
    //     BstClear(testFFTArr1->arr, NULL, 0);
    //     free(testFFTArr1);
    //     BstClear(testFFTArr2->arr, NULL, 0);
    //     free(testFFTArr2);
    // }
    printf("a");
}