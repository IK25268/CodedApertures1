#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FFT2D.h"
#include "Complex.h"
#include "Matrix.h"
#include <string.h>

unsigned int CalcBitRevInt(unsigned int inputInt, unsigned int size)
{
    unsigned int check = 0x01;
    unsigned int paste = pow(2, size - 1);
    unsigned int filled = 0x00;
    while (size > 0)
    {
        if ((inputInt & check) != 0)
        {
            filled |= paste;
        }
        paste = paste >> 1;
        check = check << 1;
        size--;
    }
    return filled;
}

unsigned int CalcP(unsigned int k, unsigned int gamma,  unsigned int l)
{
    k = k >> (gamma - l);
    return CalcBitRevInt(k, gamma);
}

//----

void FFTLin(void* linArr)
{
    LinArr* input = (LinArr*) linArr;
    unsigned int gamma = log2(input->lin);
    for (unsigned int l = 1; l <= gamma; l++)
    {
        unsigned int size1 = pow(2, gamma - l);
        unsigned int currRevPos = 0;
        unsigned int currRevPosPair = size1;
        for (unsigned int countGroup = 0; countGroup < pow(2, l - 1); countGroup++)
        {
            unsigned int p = CalcBitRevInt(countGroup, gamma - 1);
            Compl Wp = {cos((2*M_PI*p)/(input->lin)), -sin((2*M_PI*p)/(input->lin))};   
              
            for ( unsigned int i = 0; i < size1; currRevPos++, currRevPosPair++, i++ )
            {
                //Compl* num2 = (Compl*)BstFind(input->arr, &currRevPosPair);
                Compl multipl = MulCC( (Compl*)BstFind(input->arr, &currRevPosPair) , &Wp );
                //Compl* num2AFTERMULT = (Compl*)BstFind(columns[currCalc]->arr, &currRevPosPair);
                //printf("a");

                //Compl* num1 = (Compl*)BstFind(columns[currCalc]->arr, &currRevPos);
                *(Compl*)BstFind(input->arr, &currRevPosPair) = SubCC((Compl*)BstFind(input->arr, &currRevPos), &multipl);
                *(Compl*)BstFind(input->arr, &currRevPos) = AddCC((Compl*)BstFind(input->arr, &currRevPos), &multipl);
                //Compl* numtargetAFTERADD = (Compl*)BstFind(input->arr, &currRevPos);           
                //printf("b");            

                
                //Compl* numtargetPairAFTERSUB = (Compl*)BstFind(input->arr, &currRevPosPair); 
                //printf("c");
            }
            currRevPos += size1 ;
            currRevPosPair += size1 ;
        }
    }
    
    BitReverse1(input);
   // free(columns[currCalc]);
    
    //printf(" aa ");
}

void RevFFTLin(void* linArr)
{
    LinArr* input = (LinArr*) linArr;
    unsigned int gamma = log2(input->lin);
    for (unsigned int l = 1; l <= gamma; l++)
    {
        unsigned int size1 = pow(2, gamma - l);
        unsigned int currRevPos = 0;
        unsigned int currRevPosPair = size1;
        for (unsigned int countGroup = 0; countGroup < pow(2, l - 1); countGroup++)
        {
            unsigned int p = CalcBitRevInt(countGroup, gamma - 1);
            Compl Wp = {cos((2*M_PI*p)/(input->lin)), sin((2*M_PI*p)/(input->lin))};   
              
            for ( unsigned int i = 0; i < size1; currRevPos++, currRevPosPair++, i++ )
            {
                //Compl* num2 = (Compl*)BstFind(input->arr, &currRevPosPair);
                Compl multipl = MulCC( (Compl*)BstFind(input->arr, &currRevPosPair) , &Wp );
                //Compl* num2AFTERMULT = (Compl*)BstFind(columns[currCalc]->arr, &currRevPosPair);
                //printf("a");

                //Compl* num1 = (Compl*)BstFind(columns[currCalc]->arr, &currRevPos);
                *(Compl*)BstFind(input->arr, &currRevPosPair) = SubCC((Compl*)BstFind(input->arr, &currRevPos), &multipl);
                *(Compl*)BstFind(input->arr, &currRevPos) = AddCC((Compl*)BstFind(input->arr, &currRevPos), &multipl);
                //Compl* numtargetAFTERADD = (Compl*)BstFind(input->arr, &currRevPos);           
                //printf("b");            

                
                //Compl* numtargetPairAFTERSUB = (Compl*)BstFind(input->arr, &currRevPosPair); 
                //printf("c");
            }
            currRevPos += size1 ;
            currRevPosPair += size1 ;
        }
    }
    
    BitReverse1(input);
}

void BitReverse1(void* linArrTgt)
{
    LinArr* tgt = (LinArr*) linArrTgt;
    List* itemList = ListCreate(tgt->arr->cmpFunc, sizeof(void*));
	Append(itemList, &(tgt->arr->root));
    unsigned int sizeKey = log2(tgt->lin);
    Tree* usedRev = CreateUsedRevKeys();
    while (Length(itemList) != 0)
	{
        TreeItem* treeItem = *(void**)Get(itemList, 0);
        unsigned int pos = *((unsigned int*)(treeItem->key));
        if (BstFindItem(usedRev, &pos)==NULL)
        {
            unsigned int posBitRev = CalcBitRevInt(pos, sizeKey);
            char a = 0;
            BstInsert(usedRev, NULL, &a, &posBitRev);

            Compl* from = (Compl*)(BstFind(tgt->arr, &posBitRev));
            Compl buf = *from;
            *from = *((Compl*)(treeItem->data));
            *((Compl*)(treeItem->data)) = buf;
        }
        if (treeItem->less->data != NULL)
		{
			Append(itemList, &(treeItem->less));
		}
		if (treeItem->more->data != NULL)
		{
			Append(itemList, &(treeItem->more));
		}
		Remove(itemList, 0);
    }
    BstDestroy(usedRev, NULL, 0);
}

void BitReverse(void* linArrTgt, void* linArrSrc, unsigned int size)
{
    Tree* tgt = (Tree*) linArrTgt;
    Tree* src = (Tree*) linArrSrc;
    List* itemList = ListCreate(src->cmpFunc, sizeof(void*));
	Append(itemList, &(src->root));
    unsigned int sizeKey = log2(size);
    while (Length(itemList) != 0)
	{
        TreeItem* treeItem = *(void**)Get(itemList, 0);
        unsigned int pos = *((unsigned int*)(treeItem->key));
        unsigned int posBitRev = CalcBitRevInt(pos, sizeKey);
        *((Compl*)(BstFind(tgt, &posBitRev))) = *((Compl*)(treeItem->data));
        if (treeItem->less->data != NULL)
		{
			Append(itemList, &(treeItem->less));
		}
		if (treeItem->more->data != NULL)
		{
			Append(itemList, &(treeItem->more));
		}
		Remove(itemList, 0);
    }
}

//----------------------2D

void FFTLaunchMatrix(void* matrix)
{
    Matrix* input  = (Matrix*)matrix;
    List* itemListRows = ListCreate(input->Pixels->cmpFunc, sizeof(void*));
    Append(itemListRows, &(input->Pixels->root));
    while (Length(itemListRows) != 0)
    {
        TreeItem* treeItemRow = *(void**)Get(itemListRows, 0);
        Tree* treeY = treeItemRow->data;
        FFTTreeRow(treeY, input->cols);
        if (treeItemRow->less->data != NULL)
        {
            Append(itemListRows, &(treeItemRow->less));
        }
        if (treeItemRow->more->data != NULL)
        {
            Append(itemListRows, &(treeItemRow->more));
        }
        Remove(itemListRows, 0);
    }
    for (unsigned int col = 0; col < input->cols; col++)
    {
        FFTTreeCol(input->Pixels, &col, input->rows);
    }
}

void FFTTreeRow(void* treeRow, unsigned int cols)
{
    unsigned int gamma = log2(cols);
    for (unsigned int l = 1; l <= gamma; l++)
    {
        unsigned int size1 = pow(2, gamma - l);
        unsigned int currRevPos = 0;
        unsigned int currRevPosPair = size1;
        for (unsigned int countGroup = 0; countGroup < pow(2, l - 1); countGroup++)
        {
            unsigned int p = CalcBitRevInt(countGroup, gamma - 1);
            Compl Wp = {cos((2*M_PI*p)/(cols)), -sin((2*M_PI*p)/(cols))};   
            for ( unsigned int i = 0; i < size1; currRevPos++, currRevPosPair++, i++ )
            {
                Compl multipl = MulCC( (Compl*)BstFind(treeRow, &currRevPosPair) , &Wp );
                *(Compl*)BstFind(treeRow, &currRevPosPair) = SubCC((Compl*)BstFind(treeRow, &currRevPos), &multipl);
                *(Compl*)BstFind(treeRow, &currRevPos) = AddCC((Compl*)BstFind(treeRow, &currRevPos), &multipl);
            }
            currRevPos += size1 ;
            currRevPosPair += size1 ;
        }
    }
    
    BitReverseRow(treeRow, cols);
}
void FFTTreeCol(void* tree, unsigned int *col, unsigned int rows)
{
    unsigned int gamma = log2(rows);
    for (unsigned int l = 1; l <= gamma; l++)
    {
        unsigned int size1 = pow(2, gamma - l);
        unsigned int currRevPos = 0;
        unsigned int currRevPosPair = size1;
        for (unsigned int countGroup = 0; countGroup < pow(2, l - 1); countGroup++)
        {
            unsigned int p = CalcBitRevInt(countGroup, gamma - 1);
            Compl Wp = {cos((2*M_PI*p)/(rows)), -sin((2*M_PI*p)/(rows))};   
            for ( unsigned int i = 0; i < size1; currRevPos++, currRevPosPair++, i++ )
            {
                Compl multipl = MulCC( (Compl*)BstFind(BstFind(tree, col), &currRevPosPair) , &Wp );
                *(Compl*)BstFind(BstFind(tree, col), &currRevPosPair) = SubCC((Compl*)BstFind(BstFind(tree, col), &currRevPos), &multipl);
                *(Compl*)BstFind(BstFind(tree, col), &currRevPos) = AddCC((Compl*)BstFind(BstFind(tree, col), &currRevPos), &multipl);
            }
            currRevPos += size1 ;
            currRevPosPair += size1 ;
        }
    }
    
    BitReverseCol(tree, col, rows);
}

void RevFFTLaunchMatrix(void* matrix)
{
    Matrix* input  = (Matrix*)matrix;
    List* itemListRows = ListCreate(input->Pixels->cmpFunc, sizeof(void*));
    Append(itemListRows, &(input->Pixels->root));
    while (Length(itemListRows) != 0)
    {
        TreeItem* treeItemRow = *(void**)Get(itemListRows, 0);
        Tree* treeY = treeItemRow->data;
        RevFFTTreeRow(treeY, input->cols);
        if (treeItemRow->less->data != NULL)
        {
            Append(itemListRows, &(treeItemRow->less));
        }
        if (treeItemRow->more->data != NULL)
        {
            Append(itemListRows, &(treeItemRow->more));
        }
        Remove(itemListRows, 0);
    }
    for (unsigned int col = 0; col < input->cols; col++)
    {
        RevFFTTreeCol(input->Pixels, &col, input->rows);
    }
}

void RevFFTTreeRow(void* treeRow, unsigned int cols)
{
        unsigned int gamma = log2(cols);
    for (unsigned int l = 1; l <= gamma; l++)
    {
        unsigned int size1 = pow(2, gamma - l);
        unsigned int currRevPos = 0;
        unsigned int currRevPosPair = size1;
        for (unsigned int countGroup = 0; countGroup < pow(2, l - 1); countGroup++)
        {
            unsigned int p = CalcBitRevInt(countGroup, gamma - 1);
            Compl Wp = {cos((2*M_PI*p)/(cols)), sin((2*M_PI*p)/(cols))};   
            for ( unsigned int i = 0; i < size1; currRevPos++, currRevPosPair++, i++ )
            {
                Compl multipl = MulCC( (Compl*)BstFind(treeRow, &currRevPosPair) , &Wp );
                *(Compl*)BstFind(treeRow, &currRevPosPair) = SubCC((Compl*)BstFind(treeRow, &currRevPos), &multipl);
                *(Compl*)BstFind(treeRow, &currRevPos) = AddCC((Compl*)BstFind(treeRow, &currRevPos), &multipl);
            }
            currRevPos += size1 ;
            currRevPosPair += size1 ;
        }
    }
    
    BitReverseRow(treeRow, cols);
}
void RevFFTTreeCol(void* tree, unsigned int *col, unsigned int rows)
{
    unsigned int gamma = log2(rows);
    for (unsigned int l = 1; l <= gamma; l++)
    {
        unsigned int size1 = pow(2, gamma - l);
        unsigned int currRevPos = 0;
        unsigned int currRevPosPair = size1;
        for (unsigned int countGroup = 0; countGroup < pow(2, l - 1); countGroup++)
        {
            unsigned int p = CalcBitRevInt(countGroup, gamma - 1);
            Compl Wp = {cos((2*M_PI*p)/(rows)), sin((2*M_PI*p)/(rows))};   
            for ( unsigned int i = 0; i < size1; currRevPos++, currRevPosPair++, i++ )
            {
                Compl multipl = MulCC( (Compl*)BstFind(BstFind(tree, col), &currRevPosPair) , &Wp );
                *(Compl*)BstFind(BstFind(tree, col), &currRevPosPair) = SubCC((Compl*)BstFind(BstFind(tree, col), &currRevPos), &multipl);
                *(Compl*)BstFind(BstFind(tree, col), &currRevPos) = AddCC((Compl*)BstFind(BstFind(tree, col), &currRevPos), &multipl);
            }
            currRevPos += size1 ;
            currRevPosPair += size1 ;
        }
    }
    
    BitReverseCol(tree, col, rows);
}

void BitReverseRow(void* treeRow, unsigned int cols)
{
   Tree* tgt = (Tree*) treeRow;
    List* itemList = ListCreate(tgt->cmpFunc, sizeof(void*));
	Append(itemList, &(tgt->root));
    unsigned int sizeKey = log2(cols);
    char a = 0;
    Tree* usedRev = CreateUsedRevKeys();
    while (Length(itemList) != 0)
	{
        TreeItem* treeItem = *(void**)Get(itemList, 0);
        unsigned int pos = *((unsigned int*)(treeItem->key));
        if (BstFindItem(usedRev, &pos)==NULL)
        {
            unsigned int posBitRev = CalcBitRevInt(pos, sizeKey);
            
            BstInsert(usedRev, NULL, &a, &posBitRev);

            Compl* from = (Compl*)(BstFind(tgt, &posBitRev));
            Compl buf = *from;
            *from = *((Compl*)(treeItem->data));
            *((Compl*)(treeItem->data)) = buf;
        }
        if (treeItem->less->data != NULL)
		{
			Append(itemList, &(treeItem->less));
		}
		if (treeItem->more->data != NULL)
		{
			Append(itemList, &(treeItem->more));
		}
		Remove(itemList, 0);
    }
    BstDestroy(usedRev, NULL, 0);
}
void BitReverseCol(void* tree, unsigned int *col, unsigned int rows)
{
   Tree* tgt = (Tree*) tree;
    unsigned int sizeKey = log2(rows);
    Tree* usedRev = CreateUsedRevKeys();
    char a = 0;
    for (unsigned int row = 0; row < rows; row++)
    {
        if (BstFindItem(usedRev, &row)==NULL)
        {
            unsigned int posBitRev = CalcBitRevInt(row, sizeKey);
            BstInsert(usedRev, NULL, &a, &posBitRev);

            Compl* from = BstFind(BstFind(tree, &row), col);            
            Compl* to = BstFind(BstFind(tree, &posBitRev), col);  
            Compl buf = *from;
            *from = *to;
            *to = buf;
        }
    }
    BstDestroy(usedRev, NULL, 0);
}