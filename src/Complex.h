#pragma once

typedef struct Compl {
	float real;
	float imag;
}Compl;

Compl NullC();
Compl AddCC(void* a, void* b);
Compl SubCC(void* a, void* b);
Compl MulCC(void* a, void* b);
Compl DivCC(void* a, void* b);
void AddCC1(void* a, void* b);
void SubCC1(void* a, void* b);
void MulCC1(void* a, void* b);
void DivCC1(void* a, void* b);
void ConjC(void* a);
void AbsC(void* a);
void SqAbsC(void* a);
