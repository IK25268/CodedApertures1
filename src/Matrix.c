#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Matrix.h"
#include <string.h>
#include "Complex.h"

int CmpFuncInt(void* inputData1, void* inputData2)
{
	unsigned int* a = (unsigned int*)inputData1;
	unsigned int* b = (unsigned int*)inputData2;
	if (*a == *b)
	{
		return 2;
	}
	else
	{
		if (*a < *b)
			return 1;
		if (*a > *b)
			return 3;
	}
	return 0;
}

LinArr* CreateLinArr(unsigned int len)
{
    LinArr* arrLin = (LinArr*)malloc(sizeof(LinArr));
    arrLin->lin = len;
    arrLin->arr = BstCreate(CmpFuncInt, sizeof(Compl));
    return arrLin;
}

Tree* CreateBufer()
{
    return BstCreate(CmpFuncInt, sizeof(Compl));
}

Tree* CreateUsedRevKeys()
{
    return BstCreate(CmpFuncInt, sizeof(char));
}

void MultFreqLin(LinArr* arrTgt, LinArr* arrSrc)
{
    List* arrSrcList = ListCreate(arrSrc->arr->cmpFunc, sizeof(void*));
    Append(arrSrcList, &(arrSrc->arr->root));
    List* arrTgtList = ListCreate(arrTgt->arr->cmpFunc, sizeof(void*));
    Append(arrTgtList, &(arrTgt->arr->root));
    while (Length(arrSrcList) != 0)
    {
        TreeItem* treeItemSrc = *(void**)Get(arrSrcList, 0);
        Compl* b = treeItemSrc->data;
        TreeItem* treeItemTgt = *(void**)Get(arrTgtList, 0);
        Compl* a = treeItemTgt->data;
        
        Compl buf = {a->real*b->real - a->imag*b->imag, a->imag*b->real + a->real*b->imag};
        a->real = buf.real;
        a->imag = buf.imag;
        if (treeItemSrc->less->data != NULL)
        {
            Append(arrSrcList, &(treeItemSrc->less));
            Append(arrTgtList, &(treeItemTgt->less));
        }
        if (treeItemTgt->more->data != NULL)
        {
            Append(arrSrcList, &(treeItemSrc->more));
            Append(arrTgtList, &(treeItemTgt->more));
        }
        Remove(arrSrcList, 0);
        Remove(arrTgtList, 0);
    }
}
void ConvLin(LinArr* arrTgt, LinArr* arrSrc1, LinArr* arrSrc2)
{
    List* arrTgtList = ListCreate(arrTgt->arr->cmpFunc, sizeof(void*));
    Append(arrTgtList, &(arrTgt->arr->root));
    while (Length(arrTgtList) != 0)
    {
        TreeItem* treeItemTgt = *(void**)Get(arrTgtList, 0);
        float* tgtRealSumm = &(((Compl*)(treeItemTgt->data))->real);

        List* arrSrc2List = ListCreate(arrSrc2->arr->cmpFunc, sizeof(void*));
        Append(arrSrc2List, &(arrSrc2->arr->root));
        while (Length(arrSrc2List) != 0)
        {
            TreeItem* treeItemSrc2 = *(void**)Get(arrSrc2List, 0);
            unsigned int pos = *((unsigned int*)(treeItemTgt->key)) - *((unsigned int*)(treeItemSrc2->key));
            if ((pos>=0)&&(pos<=(arrTgt->lin)))
            {
                float a = ((Compl*)(BstFind(arrSrc1->arr, &pos)))->real;
                float b = ((Compl*)(treeItemSrc2->data))->real;
                *tgtRealSumm += a * b;
            }
            if (treeItemSrc2->less->data != NULL)
            {
                Append(arrSrc2List, &(treeItemSrc2->less));
            }
            if (treeItemSrc2->more->data != NULL)
            {
                Append(arrSrc2List, &(treeItemSrc2->more));
            }
            Remove(arrSrc2List, 0);
        }

        if (treeItemTgt->less->data != NULL)
        {
            Append(arrTgtList, &(treeItemTgt->less));
        }
        if (treeItemTgt->more->data != NULL)
        {
            Append(arrTgtList, &(treeItemTgt->more));
        }
        Remove(arrTgtList, 0);
    }
}

Matrix* MatrixInit(unsigned int cols, unsigned int rows)
{
    Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
    matrix->cols = cols;
    matrix->rows = rows;
    matrix->Pixels = BstCreate(CmpFuncInt, sizeof(Tree));
    return matrix;
}

void* ColsCreate()
{
    return BstCreate(CmpFuncInt, sizeof(Compl));
}

Matrix* MatrixCreate(unsigned int cols, unsigned int rows)
{
    Matrix* matrix = MatrixInit(cols, rows);
    for (unsigned int y = 0; y < rows; y++)
    {
        BstInsert(matrix->Pixels, NULL, BstCreate(CmpFuncInt, sizeof(Compl)), &y); 
    }
    return matrix;
}

void CopyMathFill(Matrix* targetEmpty, Matrix* source, void (*foreachFunc)(void* data))
{
    List* SourceTreeListY = ListCreate(source->Pixels->cmpFunc, sizeof(void*));
    Append(SourceTreeListY, &(source->Pixels->root));
    List* TargetTreeListY = ListCreate(targetEmpty->Pixels->cmpFunc, sizeof(void*));
    Append(TargetTreeListY, &(targetEmpty->Pixels->root));
    while (Length(SourceTreeListY) != 0)
    {
        TreeItem* SourceTreeItemY = *(void**)Get(SourceTreeListY, 0);
        TreeItem* TargetTreeItemY = *(void**)Get(TargetTreeListY, 0);
        Tree* SourceTreeX = SourceTreeItemY->data;
        Tree* TargetTreeX = TargetTreeItemY->data;
        List* SourceTreeListX = ListCreate(SourceTreeX->cmpFunc, sizeof(void*));
        Append(SourceTreeListX, &(SourceTreeX->root));
        while (Length(SourceTreeListX) != 0)
        {
            TreeItem* SourceTreeItemX = *(void**)Get(SourceTreeListX, 0);
            foreachFunc(BstInsert(TargetTreeX, NULL, SourceTreeItemX->data, SourceTreeItemX->key));
            if (SourceTreeItemX->less->data != NULL)
            {
                Append(SourceTreeListX, &SourceTreeItemX->less);
            }
            if (SourceTreeItemX->more->data != NULL)
            {
                Append(SourceTreeListX, &SourceTreeItemX->more);
            }
            Remove(SourceTreeListX, 0);
        }
        if (SourceTreeItemY->less->data != NULL)
        {
            Append(SourceTreeListY, &(SourceTreeItemY->less));
            Append(TargetTreeListY, &(TargetTreeItemY->less));
        }
        if (SourceTreeItemY->more->data != NULL)
        {
            Append(SourceTreeListY, &(SourceTreeItemY->more));
            Append(TargetTreeListY, &(TargetTreeItemY->more));
        }
        Remove(SourceTreeListY, 0);
        Remove(TargetTreeListY, 0);
    }
}

void Math( Matrix* source, void (*foreachFunc)(void* data))
{
    List* itemListY = ListCreate(source->Pixels->cmpFunc, sizeof(void*));
    Append(itemListY, &(source->Pixels->root));
    while (Length(itemListY) != 0)
    {
        TreeItem* treeItemY = *(void**)Get(itemListY, 0);
        Tree* treeY = treeItemY->data;
        BstForeachListPtrData(treeY, foreachFunc);
    
        if (treeItemY->less->data != NULL)
        {
            Append(itemListY, &(treeItemY->less));
        }
        if (treeItemY->more->data != NULL)
        {
            Append(itemListY, &(treeItemY->more));
        }
        Remove(itemListY, 0);
        printf("\n");
    }
}

void MathExtra( Matrix* source, void (*foreachFunc)(void* data, void* extraData), void* extraData)
{
    List* itemListY = ListCreate(source->Pixels->cmpFunc, sizeof(void*));
    Append(itemListY, &(source->Pixels->root));
    while (Length(itemListY) != 0)
    {
        TreeItem* treeItemY = *(void**)Get(itemListY, 0);
        Tree* treeY = treeItemY->data;
        BstForeachListPtrData2(treeY, foreachFunc, extraData);
    
        if (treeItemY->less->data != NULL)
        {
            Append(itemListY, &(treeItemY->less));
        }
        if (treeItemY->more->data != NULL)
        {
            Append(itemListY, &(treeItemY->more));
        }
        Remove(itemListY, 0);
        printf("\n");
    }
}

void MathTgtSrc( Matrix* target, Matrix* source, void (*foreachFunc)(void* data, void* extraData))
{
    List* SourceTreeListY = ListCreate(source->Pixels->cmpFunc, sizeof(void*));
    Append(SourceTreeListY, &(source->Pixels->root));
    List* TargetTreeListY = ListCreate(target->Pixels->cmpFunc, sizeof(void*));
    Append(TargetTreeListY, &(target->Pixels->root));
    while (Length(SourceTreeListY) != 0)
    {
        TreeItem* SourceTreeItemY = *(void**)Get(SourceTreeListY, 0);
        TreeItem* TargetTreeItemY = *(void**)Get(TargetTreeListY, 0);
        Tree* SourceTreeX = SourceTreeItemY->data;
        Tree* TargetTreeX = TargetTreeItemY->data;
        List* SourceTreeListX = ListCreate(SourceTreeX->cmpFunc, sizeof(void*));
        List* TargetTreeListX = ListCreate(TargetTreeX->cmpFunc, sizeof(void*));
        Append(SourceTreeListX, &(SourceTreeX->root));
        Append(TargetTreeListX, &(TargetTreeX->root));
        while (Length(SourceTreeListX) != 0)
        {
            TreeItem* SourceTreeItemX = *(void**)Get(SourceTreeListX, 0);
            TreeItem* TargetTreeItemX = *(void**)Get(TargetTreeListX, 0);
            foreachFunc(TargetTreeItemX->data, SourceTreeItemX->data);
            if (SourceTreeItemX->less->data != NULL)
            {
                Append(SourceTreeListX, &SourceTreeItemX->less);
                Append(TargetTreeListX, &SourceTreeItemX->less);
            }
            if (SourceTreeItemX->more->data != NULL)
            {
                Append(SourceTreeListX, &SourceTreeItemX->more);
                Append(TargetTreeListX, &SourceTreeItemX->more);
            }
            Remove(SourceTreeListX, 0);
            Remove(TargetTreeListX, 0);
        }
        if (SourceTreeItemY->less->data != NULL)
        {
            Append(SourceTreeListY, &(SourceTreeItemY->less));
            Append(TargetTreeListY, &(TargetTreeItemY->less));
        }
        if (SourceTreeItemY->more->data != NULL)
        {
            Append(SourceTreeListY, &(SourceTreeItemY->more));
            Append(TargetTreeListY, &(TargetTreeItemY->more));
        }
        Remove(SourceTreeListY, 0);
        Remove(TargetTreeListY, 0);
    }
}

void MultiplFFTTgtSrc( Matrix* target, Matrix* source)
{
    List* SourceTreeListY = ListCreate(source->Pixels->cmpFunc, sizeof(void*));
    Append(SourceTreeListY, &(source->Pixels->root));
    List* TargetTreeListY = ListCreate(target->Pixels->cmpFunc, sizeof(void*));
    Append(TargetTreeListY, &(target->Pixels->root));
    while (Length(SourceTreeListY) != 0)
    {
        TreeItem* SourceTreeItemY = *(void**)Get(SourceTreeListY, 0);
        TreeItem* TargetTreeItemY = *(void**)Get(TargetTreeListY, 0);
        Tree* SourceTreeX = SourceTreeItemY->data;
        Tree* TargetTreeX = TargetTreeItemY->data;
        List* SourceTreeListX = ListCreate(SourceTreeX->cmpFunc, sizeof(void*));
        List* TargetTreeListX = ListCreate(TargetTreeX->cmpFunc, sizeof(void*));
        Append(SourceTreeListX, &(SourceTreeX->root));
        Append(TargetTreeListX, &(TargetTreeX->root));
        while (Length(SourceTreeListX) != 0)
        {
            TreeItem* SourceTreeItemX = *(void**)Get(SourceTreeListX, 0);
            TreeItem* TargetTreeItemX = *(void**)Get(TargetTreeListX, 0);
            Compl* b = SourceTreeItemX->data;
            Compl* a = TargetTreeItemX->data;
            Compl buf = {a->real*b->real - a->imag*b->imag, a->imag*b->real + a->real*b->imag};
            a->real = buf.real;
            a->imag = buf.imag;
            if (SourceTreeItemX->less->data != NULL)
            {
                Append(SourceTreeListX, &SourceTreeItemX->less);
                Append(TargetTreeListX, &SourceTreeItemX->less);
            }
            if (SourceTreeItemX->more->data != NULL)
            {
                Append(SourceTreeListX, &SourceTreeItemX->more);
                Append(TargetTreeListX, &SourceTreeItemX->more);
            }
            Remove(SourceTreeListX, 0);
            Remove(TargetTreeListX, 0);
        }
        if (SourceTreeItemY->less->data != NULL)
        {
            Append(SourceTreeListY, &(SourceTreeItemY->less));
            Append(TargetTreeListY, &(TargetTreeItemY->less));
        }
        if (SourceTreeItemY->more->data != NULL)
        {
            Append(SourceTreeListY, &(SourceTreeItemY->more));
            Append(TargetTreeListY, &(TargetTreeItemY->more));
        }
        Remove(SourceTreeListY, 0);
        Remove(TargetTreeListY, 0);
    }
}

void NormalMatrix(Matrix* source)
{
    float max = 0.0;

    List* SourceTreeListY = ListCreate(source->Pixels->cmpFunc, sizeof(void*));
    Append(SourceTreeListY, &(source->Pixels->root));
    while (Length(SourceTreeListY) != 0)
    {
        TreeItem* SourceTreeItemY = *(void**)Get(SourceTreeListY, 0);
        Tree* SourceTreeX = SourceTreeItemY->data;
        List* SourceTreeListX = ListCreate(SourceTreeX->cmpFunc, sizeof(void*));
        Append(SourceTreeListX, &(SourceTreeX->root));
        while (Length(SourceTreeListX) != 0)
        {
            TreeItem* SourceTreeItemX = *(void**)Get(SourceTreeListX, 0);
            Compl* a = SourceTreeItemX->data;
            if (a->real > max)
            {
                max = a->real;
            }
            a->imag = 0.0;
            if (SourceTreeItemX->less->data != NULL)
            {
                Append(SourceTreeListX, &SourceTreeItemX->less);
            }
            if (SourceTreeItemX->more->data != NULL)
            {
                Append(SourceTreeListX, &SourceTreeItemX->more);
            }
            Remove(SourceTreeListX, 0);
        }
        if (SourceTreeItemY->less->data != NULL)
        {
            Append(SourceTreeListY, &(SourceTreeItemY->less));
        }
        if (SourceTreeItemY->more->data != NULL)
        {
            Append(SourceTreeListY, &(SourceTreeItemY->more));
        }
        Remove(SourceTreeListY, 0);
    }

    float coeff = 255.0/max;

    List* SourceTreeListY2 = ListCreate(source->Pixels->cmpFunc, sizeof(void*));
    Append(SourceTreeListY2, &(source->Pixels->root));
    while (Length(SourceTreeListY2) != 0)
    {
        TreeItem* SourceTreeItemY2 = *(void**)Get(SourceTreeListY2, 0);
        Tree* SourceTreeX2 = SourceTreeItemY2->data;
        List* SourceTreeListX2 = ListCreate(SourceTreeX2->cmpFunc, sizeof(void*));
        Append(SourceTreeListX2, &(SourceTreeX2->root));
        while (Length(SourceTreeListX2) != 0)
        {
            TreeItem* SourceTreeItemX2 = *(void**)Get(SourceTreeListX2, 0);
            Compl* b = SourceTreeItemX2->data;
            b->real = (int)((b->real) * coeff);
            if (SourceTreeItemX2->less->data != NULL)
            {
                Append(SourceTreeListX2, &SourceTreeItemX2->less);
            }
            if (SourceTreeItemX2->more->data != NULL)
            {
                Append(SourceTreeListX2, &SourceTreeItemX2->more);
            }
            Remove(SourceTreeListX2, 0);
        }
        if (SourceTreeItemY2->less->data != NULL)
        {
            Append(SourceTreeListY2, &(SourceTreeItemY2->less));
        }
        if (SourceTreeItemY2->more->data != NULL)
        {
            Append(SourceTreeListY2, &(SourceTreeItemY2->more));
        }
        Remove(SourceTreeListY2, 0);
    }
}

void PrintMatrix(Matrix* matrix)
{
    printf("\n");
    List* itemListY = ListCreate(matrix->Pixels->cmpFunc, sizeof(void*));
    Append(itemListY, &(matrix->Pixels->root));
    while (Length(itemListY) != 0)
    {
        TreeItem* treeItemY = *(void**)Get(itemListY, 0);
        Tree* treeY = treeItemY->data;
        List* itemListX = ListCreate(treeY->cmpFunc, sizeof(void*));
        Append(itemListX, &(treeY->root));
            while (Length(itemListX) != 0)
            {
                TreeItem* treeItemX = *(void**)Get(itemListX, 0);
                Compl* compl = treeItemX->data;
                printf("(%.2f, %.2fi) ", compl->real, compl->imag);
                if (treeItemX->less->data != NULL)
                {
                    Append(itemListX, &(treeItemX->less));
                }
                if (treeItemX->more->data != NULL)
                {
                    Append(itemListX, &(treeItemX->more));
                }
                Remove(itemListX, 0);
            
            }

        if (treeItemY->less->data != NULL)
        {
            Append(itemListY, &(treeItemY->less));
        }
        if (treeItemY->more->data != NULL)
        {
            Append(itemListY, &(treeItemY->more));
        }
        Remove(itemListY, 0);
        printf("\n");
    }
}

void PrintLinArr(LinArr* linArr)
{
    printf("\n");
    for (unsigned int i = 0; i < linArr->lin; i++)
    {
        Compl* compl = BstFind(linArr->arr, &i);
        printf("(%.2f, %.2fi) ", compl->real, compl->imag);
    }
    printf("\n");
}