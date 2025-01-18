#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Complex.h"
#include <string.h>

Compl NullC()
{
    return (Compl){0.0, 0.0};
}

Compl AddCC(void* a, void* b)
{

    return (Compl){ ((Compl*)a)->real + ((Compl*)b)->real, ((Compl*)a)->imag + ((Compl*)b)->imag };
}

Compl SubCC(void* a, void* b)
{
    return (Compl){ ((Compl*)a)->real - ((Compl*)b)->real, ((Compl*)a)->imag - ((Compl*)b)->imag };
}

Compl MulCC(void* a, void* b)
{
    return (Compl){ ((Compl*)a)->real * ((Compl*)b)->real - ((Compl*)a)->imag * ((Compl*)b)->imag, ((Compl*)a)->real * ((Compl*)b)->imag + ((Compl*)a)->imag * ((Compl*)b)->real };
}

Compl DivCC(void* a, void* b)
{
    float den = ((Compl*)b)->real*((Compl*)b)->real + ((Compl*)b)->imag*((Compl*)b)->imag;
    return (Compl){ (((Compl*)a)->real*((Compl*)b)->real + ((Compl*)a)->imag*((Compl*)b)->imag)/den, (((Compl*)a)->imag*((Compl*)b)->real - ((Compl*)a)->real*((Compl*)b)->imag)/den };
}

void AddCC1(void* a, void* b)
{
    Compl buf = {((Compl*)a)->real + ((Compl*)b)->real, ((Compl*)a)->imag + ((Compl*)b)->imag};
    (((Compl*)a)->real) = buf.real;
    (((Compl*)a)->imag) = buf.imag;
}
void SubCC1(void* a, void* b)
{
    Compl buf = {((Compl*)a)->real - ((Compl*)b)->real, ((Compl*)a)->imag - ((Compl*)b)->imag};
    (((Compl*)a)->real) = buf.real;
    (((Compl*)a)->imag) = buf.imag;
}
void MulCC1(void* a, void* b)
{
    Compl buf = {(((Compl*)a)->real) * (((Compl*)b)->real) - (((Compl*)a)->imag) * (((Compl*)b)->imag), (((Compl*)a)->real) * (((Compl*)b)->imag) + (((Compl*)a)->imag) * (((Compl*)b)->real)};
    (((Compl*)a)->real) = buf.real;
    (((Compl*)a)->imag) = buf.imag;

}
void DivCC1(void* a, void* b)
{
    float den = ((Compl*)b)->real*((Compl*)b)->real + ((Compl*)b)->imag*((Compl*)b)->imag;
    Compl buf = {(((Compl*)a)->real*((Compl*)b)->real + ((Compl*)a)->imag*((Compl*)b)->imag)/den, (((Compl*)a)->imag*((Compl*)b)->real - ((Compl*)a)->real*((Compl*)b)->imag)/den};
    (((Compl*)a)->real) = buf.real;
    (((Compl*)a)->imag) = buf.imag;
}

void ConjC(void* a)
{
    ((Compl*)a)->imag = -1*((Compl*)a)->imag;
}

void AbsC(void* a)
{
    SqAbsC(a);
    ((Compl*)a)->real = pow(((Compl*)a)->real,0.5);
}

void SqAbsC(void* a)
{
    ((Compl*)a)->real = ((Compl*)a)->real*((Compl*)a)->real + ((Compl*)a)->imag*((Compl*)a)->imag;
    ((Compl*)a)->imag = 0.0;
}