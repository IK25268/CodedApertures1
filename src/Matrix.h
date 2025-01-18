#pragma once
#include "RBT.h"
#include "List.h"

typedef struct LinArr {
	Tree* arr;
	unsigned int lin;
}LinArr;

Tree* CreateBufer();
Tree* CreateUsedRevKeys();
LinArr* CreateLinArr(unsigned int len);
void MultFreqLin(LinArr* arrTgt, LinArr* arrSrc);
void ConvLin(LinArr* arrTgt, LinArr* arrSrc1, LinArr* arrSrc2);


typedef struct Matrix {
    unsigned int rows;
    unsigned int cols;
	Tree* Pixels;
}Matrix;

Matrix* MatrixInit(unsigned int cols, unsigned int rows);
void* ColsCreate();
Matrix* MatrixCreate(unsigned int cols, unsigned int rows);
void CopyMathFill(Matrix* targetEmpty, Matrix* source, void (*foreachFunc)(void* data));
void Math( Matrix* source, void (*foreachFunc)(void* data));
void MathExtra( Matrix* source, void (*foreachFunc)(void* data, void* extraData), void* extraData);
void MathTgtSrc( Matrix* target, Matrix* source, void (*foreachFunc)(void* data, void* extraData));
void MultiplFFTTgtSrc( Matrix* target, Matrix* source);
void NormalMatrix(Matrix* source);
//-----------

//GUI DEBUG

void PrintMatrix(Matrix* matrix);
void PrintLinArr(LinArr* linArr);