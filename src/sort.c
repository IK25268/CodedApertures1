#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void Merge(int* array, int first, int last, int* bufer, int middle);
void MergeSort(int* array, int size);
void MergeRecursion(int* array, int first, int last, int* bufer);

void main(int argc, char** argv)
{
	int size = 0;
	printf("Set size\n");
	scanf_s("%d", &size);
	int* array = (int*)malloc(size * sizeof(int));
	if (array == NULL)
	{
		return;
	}
	printf("Enter array\n");
	for (int i = 0; i < size; i++)
	{
		scanf_s("%d", &array[i]);
	}
	MergeSort(array, size);
	printf("Print array\n");
	for (int i = 0; i < size; i++)
	{
		printf("%d ", array[i]);
	}
	free(array);
	printf("\nEND");
	return;
}

void MergeSort(int* array, int size)
{
	int* bufer = (int*)malloc(size * sizeof(int));
	if (bufer == NULL)
	{
		return;
	}
	if (array == NULL)
	{
		return;
	}
	for (int i = 0; i < size; i++)
	{
		bufer[i] = array[i];
	}
	MergeRecursion(array, 0, size, bufer);
	free(bufer);
}

void MergeRecursion(int* array, int first, int last, int* bufer)
{
	if (last - first < 2)
	{
		return;
	}
	if (bufer == NULL)
	{
		return; 
	}
	if (array == NULL)
	{
		return;
	}
	int middle = (last - first) / 2 + first;
	MergeRecursion(bufer, first, middle, array);
	MergeRecursion(bufer, middle, last, array);
	Merge(array, first, last, bufer, middle);
}


void Merge(int* array, int first, int last, int* bufer, int middle)
{
	int* array1 = array;
	int* bufer1 = bufer;
	int a = first;
	int b = middle;
	int arrayCount = first;
	while ((a < middle) && (b < last))
	{
		if (bufer1[a] <= bufer1[b])
		{
			array1[arrayCount] = bufer1[a];
			a++;
		}
		else
		{
			array1[arrayCount] = bufer1[b];
			b++;
		}
		arrayCount++;
	}
	while (a < middle)
	{
		array1[arrayCount] = bufer1[a];
		a++;
		arrayCount++;
	}
	while (b < last)
	{
		array1[arrayCount] = bufer1[b];
		b++;
		arrayCount++;
	}
}