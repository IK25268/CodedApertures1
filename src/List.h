#pragma once

typedef int (*CmpFunc)(void* inputData1, void* inputData2);

struct List;

typedef struct ListItem {
    void* value;
    struct ListItem* next;
} ListItem;

typedef struct List {
    ListItem* head;
    size_t sizeofItem;
    CmpFunc cmpFunc;
} List;

void* ListCreate(CmpFunc cmpFunc, size_t sizeofItem);
int Length(void* inputList);
void* Prepend(void* inputList, void* inputValue);
void* Get(void* inputList, size_t index);
void* Remove(void* inputList, size_t index);
void* Append(void* inputList, void* inputValue);
void* GetLast(void* inputList);
int Find(void* inputList, void* inputValue);
void* RemoveFirst(void* inputList, void* inputValue);
void* RemoveAll(void* inputList, void* inputValue);
void* Copy(void* inputList);
void* Concat(void* inputList1, void* inputList2);
void Foreach(void* inputList, void (*func)(void*));
void FreeList(void* inputList);
void* FindCustom(void* inputList, int (*predicate)(void*));


