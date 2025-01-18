#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "List.h"
#include <string.h>

void* ListCreate(CmpFunc cmpFunc, size_t sizeofItem)
{
	if (sizeofItem == 0) return NULL;
	if (sizeof(sizeofItem) != sizeof(size_t)) return NULL;
	List* list = NULL;
	while (1)
	{
		list = (List*)malloc(sizeof(List));
		if (list != NULL)break;
	}
	if (list == NULL) return NULL;
	list->sizeofItem = sizeofItem;
	list->head = NULL;
	list->cmpFunc = cmpFunc;
	return list;
}

int Length(void* inputList)
{
	if (inputList == NULL) return -1;;
	List* list = (List*)inputList;
	if (list == NULL) return -1;;
	int count = 0;
	if (list->head != NULL)
	{
		count++;
		ListItem* bufer = list->head;
		if (bufer == NULL) return 0;
		while (bufer->next != NULL)
		{
			bufer = bufer->next;
			count++;
		}
		bufer = bufer->next;
		free(bufer);
	}
	return count;
}


void* Prepend(void* inputList, void* inputValue)
{
	if (inputList == NULL) return NULL;
	List* list = (List*)inputList;
	if (list == NULL) return NULL;
	ListItem* listItem = NULL;
	while (1)
	{
		listItem = (ListItem*)malloc(sizeof(ListItem));
		if (listItem != NULL)break;
	}
	if (listItem == NULL) return NULL;
	while (1)
	{
		listItem->value = malloc(list->sizeofItem);
		if (listItem->value != NULL)break;
	}
	if (listItem->value == NULL) return NULL;
	/*listItem->value = inputValue;*/
	memcpy(listItem->value, inputValue, list->sizeofItem);
	listItem->next = list->head;
	list->head = listItem;
	return list;
}

void* Append(void* inputList, void* inputValue)
{
	if (inputList == NULL) return NULL;
	List* list = (List*)inputList;
	if (list == NULL) return NULL;
	ListItem* listItem = NULL;
	while (1)
	{
		listItem = (ListItem*)malloc(sizeof(ListItem));
		if (listItem != NULL)break;
	}
	if (listItem == NULL) return NULL;
	while (1)
	{
		listItem->value = malloc(list->sizeofItem);
		if (listItem->value != NULL)break;
	}
	if (listItem->value == NULL) return NULL;
	memcpy(listItem->value, inputValue, list->sizeofItem);
	if (list->head != NULL)
	{
		ListItem* bufer = list->head;
		if (bufer == NULL) return NULL;
		while (bufer->next != NULL)
		{
			bufer = bufer->next;
		}
		bufer->next = listItem;
		bufer = NULL;
		listItem->next = NULL;
		return list;
	}
	list->head = listItem;
	listItem->next = NULL;
	return list;
}

void* Get(void* inputList, size_t index)
{
	if (index < 0) return NULL;
	if (inputList == NULL) return NULL;
	List* list = (List*)inputList;
	if (list == NULL) return NULL;
	if (sizeof(index) != sizeof(size_t)) return NULL;
	if (list->head != NULL)
	{
		ListItem* bufer = list->head;
		if (bufer == NULL) return NULL;
		for (size_t j = 0; (j < index) && (bufer->next); j++)
		{
			bufer = bufer->next;
		}
		return bufer->value;
	}
	return NULL;
}

void* Remove(void* inputList, size_t index)
{
	if (index < 0) return NULL;
	if (inputList == NULL) return NULL;
	List* list = (List*)inputList;
	if (list == NULL) return NULL;
	if (sizeof(index) != sizeof(size_t)) return NULL;
	if ((list->head != NULL) && (
		index < Length(inputList)
		))
	{
		if (index == 0)
		{
			int length = Length(inputList);
			ListItem* bufer = list->head;
			if (bufer == NULL) return NULL;
			list->head = list->head->next;
			free(bufer->value);
			bufer->value = NULL;
			free(bufer);
			if (length == 1) 
			{
				free(list->head);
				list->head = NULL;
			}
			return list;
		}
		else
		{
			ListItem* bufer = list->head;
			if (bufer == NULL) return NULL;
			for (size_t j = 1; j < index; j++)
			{
				bufer = bufer->next;
			}
			ListItem* tmp = bufer->next;
			if (tmp == NULL) return NULL;
			bufer->next = bufer->next->next;
			free(bufer->value);
			bufer->value = NULL;
			free(tmp);
			return list;
		}
	}
	return NULL;
}

void* GetLast(void* inputList)
{
	if (inputList == NULL) return NULL;
	List* list = (List*)inputList;
	if (list == NULL) return NULL;
	if (list->head != NULL)
	{
		ListItem* bufer = list->head;
		while (bufer->next != NULL)
		{
			bufer = bufer->next;
		}
		return bufer->value;
	}
	return NULL;
}

int Find(void* inputList, void* inputValue)
{
	if (inputList == NULL) return -1;
	List* list = (List*)inputList;
	if (list == NULL) return -1;
	if (list->head != NULL)
	{
		ListItem* bufer = list->head;
		if (bufer == NULL) return -1;
		for (int i = 0; bufer->next != NULL; i++)
		{
			if (list->cmpFunc(bufer->value, inputValue))
			{
				return i;
			}
			bufer = bufer->next;
		}
	}
	return -1;
}

void* RemoveFirst(void* inputList, void* inputValue)
{
	if (inputList == NULL) return NULL;
	List* list = (List*)inputList;
	if (list == NULL) return NULL;
	if (list->head != NULL)
	{
		ListItem* bufer = list->head;
		if (bufer == NULL) return NULL;
		if (list->cmpFunc(bufer->value, inputValue))
		{
			list->head = list->head->next;
			free(bufer);
			return list;
		}
		else
		{
			for (int i = 0; bufer->next != NULL; i++)
			{
				if (list->cmpFunc(bufer->next->value, inputValue))
				{
					ListItem* bufer1 = bufer->next;
					if (bufer1 == NULL) return NULL;
					bufer->next = bufer->next->next;
					free(bufer1);
					return list;
				}
				bufer = bufer->next;
			}
		}
	}
	return list;
}

void* RemoveAll(void* inputList, void* inputValue)
{
	if (inputList == NULL) return NULL;
	List* list = (List*)inputList;
	if (list == NULL) return NULL;
	if (list->head != NULL)
	{
		int count = 0;
		while (count == 0)
		{
			count = 1;
			ListItem* bufer = list->head;
			if (bufer == NULL) return NULL;
			if (list->cmpFunc(bufer->value, inputValue))
			{
				list->head = list->head->next;
				free(bufer);
				count = 0;
			}
			else
			{
				for (int i = 0; bufer->next != NULL; i++)
				{
					if (list->cmpFunc(bufer->next->value, inputValue))
					{
						ListItem* bufer1 = bufer->next;
						if (bufer1 == NULL) return NULL;
						bufer->next = bufer->next->next;
						free(bufer1);
						count = 0;
					}
					bufer = bufer->next;
				}
			}
		}
	}
	return list;
}

void* Copy(void* inputList)
{
	if (inputList == NULL) return NULL;
	List* list = (List*)inputList;
	if (list == NULL) return NULL;
	List* listNew = ListCreate(list->cmpFunc, list->sizeofItem);
	for (int i = 0; i < Length(list); i++)
	{
		void* buf = NULL;
		while (1)
		{
			buf = malloc(sizeof(Get(list, i)));
			if (buf != NULL) break;
		}
		memcpy(buf, Get(list, i), sizeof(list->sizeofItem));
		Append(listNew, buf);
	}
	return listNew;
}

void* Concat(void* inputList1, void* inputList2)
{
	if (inputList1 == NULL) return NULL;
	if (inputList2 == NULL) return NULL;
	List* list1 = (List*)inputList1;
	if (list1 == NULL) return NULL;
	List* list2 = (List*)inputList2;
	if (list2 == NULL) return NULL;
	ListItem* bufer2 = list2->head;
	if (list1->head != NULL)
	{
		ListItem* bufer1 = list1->head;
		while (bufer1->next != NULL)
		{
			bufer1 = bufer1->next;
		}
		bufer1->next = bufer2;
	}
	else
	{
		list1->head = bufer2;
	}

	return list1;
}

void Foreach(void* inputList, void (*func)(void*))
{
	if (inputList == NULL) return;
	List* list = (List*)inputList;
	if (list == NULL) return;
	ListItem* bufer = list->head;
	if (bufer == NULL) return;
	if (list->head != NULL)
	{
		while (bufer != NULL)
		{
			ListItem* next = NULL;
			if (bufer->next != NULL)
			{
				next = bufer->next;
			}
			func(bufer->value);
			bufer = next;
		}
	}
	return;
}

void FreeList(void* inputList)
{
	if (inputList == NULL) return;
	List* list = (List*)inputList;
	if (list == NULL) return;
	ListItem* bufer = list->head;
	if (bufer == NULL) return;
	if (list->head != NULL)
	{
		while (bufer != NULL)
		{
			ListItem* next = NULL;
			if (bufer->next != NULL)
			{
				next = bufer->next;
			}
			free(bufer->value);
			free(bufer);
			bufer = next;
		}
	}
	list->head = NULL;
	free(list);
	return;
}

void* FindCustom(void* inputList, int (*predicate)(void*))
{
	if (inputList == NULL) return NULL;
	List* list = (List*)inputList;
	if (list == NULL) return NULL;
	ListItem* bufer = list->head;
	if (bufer == NULL) return NULL;
	if (bufer != NULL)
	{
		for (int i = 0; bufer != NULL; i++)
		{
			if (predicate(bufer->value)) return bufer->value;
			bufer = bufer->next;
		}
	}
	return NULL;
}