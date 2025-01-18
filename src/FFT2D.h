#pragma once

void FFTLin(void* linArr);

void RevFFTLin(void* linArr);

void BitReverse1(void* linArrTgt);
void BitReverse(void* linArrTgt, void* linArrSrc, unsigned int size);


void FFTLaunchMatrix(void* matrix);
void RevFFTLaunchMatrix(void* matrix);

void FFTTreeRow(void* treeRow, unsigned int cols);
void FFTTreeCol(void* tree, unsigned int *col, unsigned int rows);

void RevFFTTreeRow(void* treeRow, unsigned int cols);
void RevFFTTreeCol(void* tree, unsigned int *col, unsigned int rows);

void BitReverseRow(void* treeRow, unsigned int cols);
void BitReverseCol(void* tree, unsigned int *col, unsigned int rows);