#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "RWImg.h"
#include "Matrix.h"
#include "Complex.h"

void LinearFilterConvert(float dispRatio, void* apt)
{
    Matrix* aptAbsSq = MatrixCreate(((Matrix*)apt)->cols, ((Matrix*)apt)->rows);

    CopyMathFill(aptAbsSq, apt, SqAbsC);
   // PrintMatrix(aptAbsSq);
    Math(apt, ConjC);
    //PrintMatrix(apt);
    MathExtra(aptAbsSq, AddCC1, &dispRatio);
    //PrintMatrix(aptAbsSq);
    MathTgtSrc( apt, aptAbsSq, DivCC1);
   // PrintMatrix(apt);
}