#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "RBT.h"
#include "List.h"

void RecSize(TreeItem* inputItem, int* size);

void RecForeach(TreeItem* inputItem, void (*foreachFunc)(void* inputData, void* inputKey));
void RecForeach3(TreeItem* inputItem, void (*foreachFunc)(void* extraData1, void* extraData2, void* extraData3, void* inputData, void* inputKey), void* extraData1, void* extraData2, void* extraData3);

void RecFree(TreeItem* inputItem, void (*destroyData)(void* inputData));

TreeItem* CreateItem(void* inputData, void* key, int color, TreeItem* parent, size_t sizeofItem);

TreeItem* CreateLeaf(TreeItem* parent);

void SwapItem(TreeItem* inputItem1, TreeItem* inputItem2);

void RecForeachKey(Tree* inputTree, void* inputKey, void (*foreachFunc)(void* extraData1, void* extraData2, void* extraData3, void* inputData, void* inputKey), void* extraData1, void* extraData2, void* extraData3);

void LeftRotate(TreeItem* inputItem);

void RightRotate(TreeItem* inputItem);

void BalanceTree(Tree* inputTree, TreeItem* inputItem);

TreeItem* MaxSearch(Tree* inputTree, TreeItem* inputItem, TreeItem* compareItem);

TreeItem* MinSearch(Tree* inputTree, TreeItem* inputItem, TreeItem* compareItem);

TreeItem* FindBrother(TreeItem* inputItem);

void BalanceTreeDelete(Tree* inputTree, TreeItem* inputItem);

Tree* BstCreate(CmpFunc cmpFunc, size_t sizeofItem)
{
	if (cmpFunc == 0) return NULL;
	Tree* tree = NULL;
	while (1)
	{
		tree = (Tree*)malloc(sizeof(Tree));
		if (tree != NULL)break;
	}
	if (tree == NULL) return NULL;
	tree->root = NULL;
	tree->cmpFunc = cmpFunc;
	tree->sizeofItem = sizeofItem;
	return tree;
}

//void BstClear(Tree* inputTree, void (*destroyData)(void* inputData))
//{
//	Tree* tree = (Tree*)inputTree;
//	RecFree(tree->root, destroyData);
//	tree->root = NULL;
//	tree->cmpFunc = NULL;
//}

void BstClear(Tree* inputTree, void (*destroyData)(void* inputData), int flagDebug)
{
	if (inputTree == NULL) return;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return;
	if (tree->root != NULL)
	{
		List* itemList = ListCreate(tree->cmpFunc, sizeof(void*));
		Append(itemList, &tree->root);
		while (
			Length(itemList) != 0
			)
		{

			TreeItem* treeItem = *(void**)Get(itemList, 0);
			Remove(itemList, 0);
			if (treeItem->less->data != NULL)
			{
				Append(itemList, &treeItem->less);
			}
			else
			{
				free(treeItem->less);
				treeItem->less = NULL;
			}
			if (treeItem->more->data != NULL)
			{
				Append(itemList, &treeItem->more);
			}
			else
			{
				free(treeItem->more);
				treeItem->more = NULL;
			}
			if (destroyData != NULL) destroyData(treeItem->data);
			free(treeItem->key);
			treeItem->key = NULL;
			free(treeItem->data);
			treeItem->data = NULL;
			free(treeItem);
			treeItem = NULL;
			if (flagDebug) printf ("%d\n", Length(itemList));
		}
	}
	return;
}

void BstClearKey(Tree* inputTree, void (*destroyData)(void* inputData), int flagDebug)
{
	if (inputTree == NULL) return;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return;
	if (tree->root != NULL)
	{
		List* keyList = ListCreate(tree->cmpFunc, sizeof(void*));
		int key1 = *(int*)tree->root->key;
		Append(keyList, tree->root->key);
		while (
			Length(keyList) != 0
			)
		{
			void* keyItem = (void*)Get(keyList, 0);
			int key2 = *(int*)keyItem;
			TreeItem* bufItem = (TreeItem*)BstFindItem(tree, keyItem);
			//if (bufItem!=NULL)
			//{
				// if (BstSize(tree)<=100)
				// {
				// 	printf("a");
				// }
				if (bufItem->less->data != NULL)
				{
					Append(keyList, bufItem->less->key);
				}
				if (bufItem->more->data != NULL)
				{
					Append(keyList, bufItem->more->key);
				}
				if (destroyData != NULL) destroyData(bufItem);
			//}

			void* value = BstDelete(tree, keyItem);
			free(value);
			Remove(keyList, 0);
			//if (flagDebug) printf ("%d\n", Length(itemList));
		}
	}
	tree->root = NULL;
	tree->cmpFunc = NULL;
	return;
}

void BstDestroy(Tree* inputTree, void (*destroyData)(void* inputData), int flagDebug)
{
	Tree* tree = (Tree*)inputTree;
	BstClear(tree, destroyData, flagDebug);
	//free(tree);
	tree->root = NULL;
	tree->cmpFunc = NULL;
	tree->sizeofItem = 0;
}
void BstDestroyKey(Tree* inputTree, void (*destroyData)(void* inputData), int flagDebug)
{
	Tree* tree = (Tree*)inputTree;
	BstClearKey(tree, destroyData, flagDebug);
	tree->root = NULL;
	tree->cmpFunc = NULL;
	tree->sizeofItem = 0;
}

void RecForeachKey(Tree* inputTree, void* inputKey, void (*foreachFunc)(void* extraData1, void* extraData2, void* extraData3, void* inputData, void* inputKey), void* extraData1, void* extraData2, void* extraData3)
{
	TreeItem* item = BstFindItem(inputTree, inputKey);
	if (item != NULL)
	{
		void* lessKey = NULL;
		void* moreKey = NULL;
		if (item->less != NULL) lessKey = item->less->key;
		if (item->more != NULL) moreKey = item->more->key;
		foreachFunc(extraData1, extraData2, extraData3, item->data, item->key);
		if (lessKey != NULL) RecForeachKey(inputTree, lessKey, foreachFunc, extraData1, extraData2, extraData3);
		if (moreKey != NULL) RecForeachKey(inputTree, moreKey, foreachFunc, extraData1, extraData2, extraData3);
	}
}

int BstSize(Tree* inputTree)
{
	if (inputTree == NULL) return -1;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return -1;
	int size = 0;
	int* sizePtr = &size;
	if (tree->root == NULL) return 0;
	List* itemList = ListCreate(tree->cmpFunc, sizeof(int));
	Append(itemList, tree->root->key);
	size = 1;
	while (Length(itemList) != 0)
	{
		int key = *(int*)Get(itemList, 0);
		Remove(itemList, 0);
		TreeItem* treeItem = BstFindItem(tree, &key);
		if (treeItem->less->data != NULL)
		{
			size++;
			Append(itemList, treeItem->less->key);
		}
		if (treeItem->more->data != NULL)
		{
			size++;
			Append(itemList, treeItem->more->key);
		}
	}
	return size;
}

void* BstFind(Tree* inputTree, void* key)
{
	if (inputTree == NULL) return NULL;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return NULL;
	if (tree->root == NULL) return NULL;
	TreeItem* bufer = tree->root;
	int count = 0;
	while (bufer->data != NULL)
	{
		int compare = tree->cmpFunc(key, bufer->key);
		if (compare == 2)
		{
			count = 1;
			break;
		}
		else
		{
			if (compare == 1)
			{
				bufer = bufer->less;
			}
			if (compare == 3)
			{
				bufer = bufer->more;
			}
		}
	}
	if (count == 0)
	{
		return NULL;
	}
	else
	{
		return bufer->data;
	}
}

void* BstFindItem(Tree* inputTree, void* key)
{
	if (inputTree == NULL) return NULL;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return NULL;
	if (tree->root == NULL) return NULL;
	TreeItem* bufer = tree->root;
	int count = 0;
	while (bufer->data != NULL)
	{
		int compare = tree->cmpFunc(key, bufer->key);
		if (compare == 2)
		{
			count = 1;
			break;
		}
		else
		{
			if (compare == 1)
			{
				bufer = bufer->less;
			}
			if (compare == 3)
			{
				bufer = bufer->more;
			}
		}
	}
	if (count == 0)
	{
		return NULL;
	}
	else
	{
		return bufer;
	}
}

void BstForeach(Tree* inputTree, void (*foreachFunc)(void* inputData, void* inputKey))
{
	Tree* tree = (Tree*)inputTree;
	if ((tree->root != NULL) && (foreachFunc != NULL))
	{
		RecForeach(tree->root, foreachFunc);
	}
}

void BstForeach3(Tree* inputTree, void (*foreachFunc)(void* extraData1, void* extraData2, void* extraData3, void* inputData, void* inputKey), void* extraData1, void* extraData2, void* extraData3)
{
	Tree* tree = (Tree*)inputTree;
	if ((tree->root != NULL) && (foreachFunc != NULL))
	{
		RecForeach3(tree->root, foreachFunc, extraData1, extraData2, extraData3);
	}
}

void BstForeachKey(Tree* inputTree, void (*foreachFunc)(void* extraData1, void* extraData2, void* extraData3, void* inputData, void* inputKey), void* extraData1, void* extraData2, void* extraData3)
{
	Tree* tree = (Tree*)inputTree;
	if ((tree->root != NULL) && (foreachFunc != NULL))
	{
		RecForeachKey(tree, tree->root->key, foreachFunc, extraData1, extraData2, extraData3);
	}
}

void BstForeachList(Tree* inputTree, void (*foreachFunc)(void* inputData, void* inputKey))
{
	if (inputTree == NULL) return;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return;
	if (tree->root == NULL) return;
	List* itemList = ListCreate(tree->cmpFunc, sizeof(int));
	Append(itemList, tree->root->key);
	int pos = 0;
	while (pos < BstSize(tree))
	{
		TreeItem* treeItem = BstFindItem(tree, Get(itemList, pos));
		if (treeItem->less->data != NULL)
		{
			Append(itemList, treeItem->less->key);
		}
		if (treeItem->more->data != NULL)
		{
			Append(itemList, treeItem->more->key);
		}
		pos++;
	}

	while (Length(itemList) != 0)
	{
		int key2 = *(int*)Get(itemList, 0);
		TreeItem* treeItem = BstFindItem(tree, &key2);
		Remove(itemList, 0);
		foreachFunc(treeItem->data, treeItem->key);
	}
	return;
}

void BstForeachListPtrData(Tree* inputTree, void (*foreachFunc)(void* inputData))
{
	if (inputTree == NULL) return;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return;
	if (tree->root == NULL) return;
	List* itemList = ListCreate(tree->cmpFunc, sizeof(void*));
	Append(itemList, &(tree->root));
	while (Length(itemList) != 0)
	{
		TreeItem* treeItem = *(void**)Get(itemList, 0);
		foreachFunc(treeItem->data);
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

void BstForeachListPtrData2(Tree* inputTree, void (*foreachFunc)(void* inputData, void* extraData), void* extraData)
{
	if (inputTree == NULL) return;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return;
	if (tree->root == NULL) return;
	List* itemList = ListCreate(tree->cmpFunc, sizeof(void*));
	Append(itemList, &(tree->root));
	while (Length(itemList) != 0)
	{
		TreeItem* treeItem = *(void**)Get(itemList, 0);
		foreachFunc(treeItem->data, extraData);
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

void BstForeachList2(Tree* inputTree, void (*foreachFunc)(void* extraData1, void* extraData2, void* inputData, void* inputKey), void* extraData1, void* extraData2)
{
	if (inputTree == NULL) return;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return;
	if (tree->root == NULL) return;
	List* itemList = ListCreate(tree->cmpFunc, sizeof(int));
	Append(itemList, tree->root->key);
	int pos = 0;
	while (pos < BstSize(tree))
	{
		TreeItem* treeItem = BstFindItem(tree, Get(itemList, pos));
		if (treeItem->less->data != NULL)
		{
			Append(itemList, treeItem->less->key);
		}
		if (treeItem->more->data != NULL)
		{
			Append(itemList, treeItem->more->key);
		}
		pos++;

	}

	while (Length(itemList) != 0)
	{
		int key2 = *(int*)Get(itemList, 0);
		TreeItem* treeItem = BstFindItem(tree, &key2);
		Remove(itemList, 0);
		foreachFunc(extraData1, extraData2, treeItem->data, treeItem->key);
	}
	return;
}

void BstForeachListNoDelete(Tree* inputTree, void (*foreachFunc)(void* extraData1, void* extraData2, void* inputData, void* inputKey), void* extraData1, void* extraData2)
{
	if (inputTree == NULL) return;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return;
	if (tree->root == NULL) return;
	List* itemList = ListCreate(tree->cmpFunc, sizeof(int));
	Append(itemList, tree->root->key);
	while (Length(itemList) != 0)
	{
		TreeItem* treeItem = BstFindItem(tree, Get(itemList, 0));
		Remove(itemList, 0);
		foreachFunc(extraData1, extraData2, treeItem->data, treeItem->key);
		if (treeItem->less->data != NULL)
		{
			Append(itemList, treeItem->less->key);
		}
		if (treeItem->more->data != NULL)
		{
			Append(itemList, treeItem->more->key);
		}
	}
	return;
}

void* BstInsert(Tree* inputTree, void* extraData2, void* inputData, void* key)
{
	if (inputTree == NULL) return NULL;
	if (inputData == NULL) return NULL;
	if (key == NULL) return NULL;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return NULL;
	if (tree->root == NULL)
	{
		TreeItem* treeItem = CreateItem(inputData, key, 0, NULL, tree->sizeofItem);
		tree->root = treeItem;
		return treeItem->data;
	}
	else
	{
		TreeItem* bufer = tree->root;
		if (bufer->data != NULL)
		{
			while (1)
			{
				int compare = tree->cmpFunc(key, bufer->key);
				if (compare == 2)
				{
					memcpy(bufer->data, inputData, tree->sizeofItem);
					return NULL;
				}
				else
				{
					if (compare == 1)
					{
						if (bufer->less->data == NULL)
						{
							free(bufer->less);
							TreeItem* treeItem = CreateItem(inputData, key, 1, bufer, tree->sizeofItem);
							bufer->less = treeItem;
							BalanceTree(tree, bufer->less);
							return treeItem->data;
						}
						bufer = bufer->less;
					}
					if (compare == 3)
					{
						if (bufer->more->data == NULL)
						{
							free(bufer->more);
							TreeItem* treeItem = CreateItem(inputData, key, 1, bufer, tree->sizeofItem);
							bufer->more = treeItem;
							BalanceTree(tree, bufer->more);
							return treeItem->data;
						}
						bufer = bufer->more;
					}
				}
			}
		}
	}
	return NULL;
}

void RecFree(TreeItem* inputItem, void (*destroyData)(void* inputData))
{
	if (inputItem != NULL)
	{
		TreeItem* bufer = inputItem;
		if (bufer != NULL)
		{
			if (bufer->less != NULL)
				RecFree(bufer->less, destroyData);
			if (bufer->more != NULL)
				RecFree(bufer->more, destroyData);
			if (bufer->data != NULL)
			{
				if (destroyData != NULL) destroyData(bufer->data);
				free(bufer->key);
				free(bufer->data);
			}
			free(bufer);
		}
	}
}

void RecSize(TreeItem* inputItem, int* size)
{
	if (inputItem != NULL)
	{
		*size = *size + 1;
		TreeItem* bufer = inputItem;
		if (bufer->data != NULL)
		{
			if (bufer->less->data != NULL)
			{
				RecSize(bufer->less, size);
			}
			if (bufer->more->data != NULL)
			{
				RecSize(bufer->more, size);
			}
		}
	}
}

void RecForeach(TreeItem* inputItem, void (*foreachFunc)( void* inputData, void* inputKey))
{
	if (inputItem->data != NULL)
	{
		TreeItem* less = NULL;
		TreeItem* more = NULL;
		if (inputItem->less->data != NULL) less = inputItem->less;
		if (inputItem->more->data != NULL) more = inputItem->more;
		foreachFunc(inputItem->data, inputItem->key);
		if (less != NULL) RecForeach(less, foreachFunc);
		if (more != NULL) RecForeach(more, foreachFunc);
	}
}

void RecForeach3(TreeItem* inputItem, void (*foreachFunc)(void* extraData1, void* extraData2, void* extraData3, void* inputData, void* inputKey), void* extraData1, void* extraData2, void* extraData3)
{
	if (inputItem->data != NULL)
	{
		TreeItem* less = NULL;
		TreeItem* more = NULL;
		if (inputItem->less->data != NULL) less = inputItem->less;
		if (inputItem->more->data != NULL) more = inputItem->more;
		foreachFunc(extraData1, extraData2, extraData3, inputItem->data, inputItem->key);
		if (less != NULL) RecForeach3(less, foreachFunc, extraData1, extraData2, extraData3);
		if (more != NULL) RecForeach3(more, foreachFunc, extraData1, extraData2, extraData3);
	}
}

TreeItem* CreateItem(void* inputData, void* key, int color, TreeItem* parent, size_t sizeofItem)
{
	if (inputData == NULL) return NULL;
	if (key == NULL) return NULL;
	TreeItem* treeItem = NULL;
	while (1)
	{
		treeItem = (TreeItem*)malloc(sizeof(TreeItem));
		if (treeItem != NULL)break;
	}
	if (treeItem == NULL) return NULL;
	while (1)
	{
		treeItem->key = malloc(sizeof(key));
		if (treeItem->key != NULL)break;
	}
	if (treeItem->key == NULL) return NULL;
	while (1)
	{
		treeItem->data = malloc(sizeofItem);
		if (treeItem->data != NULL)break;
	}
	if (treeItem->data == NULL) return NULL;
	memcpy(treeItem->key, key, sizeof(key));
	memcpy(treeItem->data, inputData, sizeofItem);
	treeItem->color = color;
	treeItem->less = CreateLeaf(treeItem);
	treeItem->more = CreateLeaf(treeItem);
	treeItem->parent = parent;
	return treeItem;
}

TreeItem* CreateLeaf(TreeItem* parent)
{
	TreeItem* treeItem = NULL;
	while (1)
	{
		treeItem = (TreeItem*)malloc(sizeof(TreeItem));
		if (treeItem != NULL)break;
	}
	if (treeItem == NULL) return NULL;
	treeItem->key = NULL;
	treeItem->data = NULL;
	treeItem->color = 0;
	treeItem->less = NULL;
	treeItem->more = NULL;
	treeItem->parent = parent;
	return treeItem;
}

void BalanceTree(Tree* inputTree, TreeItem* inputItem)
{
	if ((inputItem != NULL) && (inputTree != NULL))
	{
		while ((inputItem->parent != NULL) && (inputItem->parent->color == 1))
		{
			if (inputItem->parent == inputItem->parent->parent->less)
			{
				TreeItem* uncleInputItem = inputItem->parent->parent->more;
				if (uncleInputItem->color == 1)
				{
					inputItem->parent->color = 0;
					uncleInputItem->color = 0;
					inputItem->parent->parent->color = 1;
					inputItem = inputItem->parent->parent;
				}
				else
				{
					if (inputItem == inputItem->parent->more)
					{
						LeftRotate(inputItem->parent);
					}
					inputItem->parent->color = 0;
					inputItem->parent->parent->color = 1;
					RightRotate(inputItem->parent->parent);
				}
			}
			else
			{
				TreeItem* uncleInputItem = inputItem->parent->parent->less;
				if (uncleInputItem->color == 1)
				{
					inputItem->parent->color = 0;
					uncleInputItem->color = 0;
					inputItem->parent->parent->color = 1;
					inputItem = inputItem->parent->parent;
				}
				else
				{
					if (inputItem == inputItem->parent->less)
					{
						RightRotate(inputItem->parent);
					}
					inputItem->parent->color = 0;
					inputItem->parent->parent->color = 1;
					LeftRotate(inputItem->parent->parent);
				}
			}
		}
		inputTree->root->color = 0;
	}
}

void SwapItem(TreeItem* inputItem1, TreeItem* inputItem2)
{
	void* keyItem1 = inputItem1->key;
	void* dataItem1 = inputItem1->data;
	int colorItem1 = inputItem1->color;
	inputItem1->color = inputItem2->color;
	inputItem2->color = colorItem1;
	inputItem1->key = inputItem2->key;
	inputItem1->data = inputItem2->data;
	inputItem2->key = keyItem1;
	inputItem2->data = dataItem1;
}

void RightRotate(TreeItem* inputItem)
{
	SwapItem(inputItem, inputItem->less);
	TreeItem* bufer = inputItem->more;
	inputItem->more = inputItem->less;
	inputItem->less = inputItem->more->less;
	inputItem->more->less = inputItem->more->more;
	inputItem->more->more = bufer;
	inputItem->less->parent = inputItem;
	inputItem->more->more->parent = inputItem->more;
}

void LeftRotate(TreeItem* inputItem)
{
	SwapItem(inputItem, inputItem->more);
	TreeItem* bufer = inputItem->less;
	inputItem->less = inputItem->more;
	inputItem->more = inputItem->less->more;
	inputItem->less->more = inputItem->less->less;
	inputItem->less->less = bufer;
	inputItem->more->parent = inputItem;
	inputItem->less->less->parent = inputItem->less;
}

void* BstDelete(Tree* inputTree, void* key)
{
	int* suchKey = key;
	if (inputTree == NULL) return NULL;
	Tree* tree = (Tree*)inputTree;
	if (tree == NULL) return NULL;
	if (key == NULL) return NULL;
	if (tree->root == NULL) return NULL;
	TreeItem* bstFind = tree->root;
	void* value = NULL;
	int count = 0;
	while (bstFind->data != NULL)
	{
		int* suchFindKey = bstFind->key;
		int compare = tree->cmpFunc(key, bstFind->key);
		if (compare == 2)
		{
			while (1)
			{
				value = malloc(tree->sizeofItem);
				if (value != NULL)break;
			}
			memcpy(value, bstFind->data, tree->sizeofItem);
			break;
		}
		else
		{
			if (compare == 1)
			{
				bstFind = bstFind->less;
				count = 1;
			}
			if (compare == 3)
			{
				bstFind = bstFind->more;
				count = 1;
			}
		}
	}
	if (bstFind->data == NULL) return NULL;
	if ((bstFind->less->data == NULL) && (bstFind->more->data == NULL))
	{
		if (count == 0)
		{
			tree->root = NULL;
		}
		else
		{
			BalanceTreeDelete(inputTree, bstFind);
			if (bstFind->parent->less == bstFind)
			{
				bstFind->parent->less = CreateLeaf(bstFind->parent);
			}
			if (bstFind->parent->more == bstFind)
			{
				bstFind->parent->more = CreateLeaf(bstFind->parent);
			}
		}
		free(bstFind->less);
		free(bstFind->more);
		free(bstFind->key);
		free(bstFind->data);
		free(bstFind);
		return value;
	}
	if ((bstFind->less->data == NULL) && (bstFind->more->data != NULL))
	{
		bstFind->more->color = 0;
		if (count == 0)
		{
			tree->root = bstFind->more;
		}
		else
		{
			if (bstFind->parent->less == bstFind)
			{
				bstFind->parent->less = bstFind->more;
				bstFind->more->parent = bstFind->parent;
			}
			if (bstFind->parent->more == bstFind)
			{
				bstFind->parent->more = bstFind->more;
				bstFind->more->parent = bstFind->parent;
			}
		}
		free(bstFind->less->data);
		free(bstFind->less->key);
		free(bstFind->key);
		free(bstFind->data);
		free(bstFind);
		return value;
	}
	if ((bstFind->less->data != NULL) && (bstFind->more->data == NULL))
	{
		bstFind->less->color = 0;
		if (count == 0)
		{
			tree->root = bstFind->less;
		}
		else
		{
			if (bstFind->parent->less == bstFind)
			{
				bstFind->parent->less = bstFind->less;
				bstFind->less->parent = bstFind->parent;
			}
			if (bstFind->parent->more == bstFind)
			{
				bstFind->parent->more = bstFind->less;
				bstFind->less->parent = bstFind->parent;
			}
		}
		free(bstFind->more->data);
		free(bstFind->more->key);
		free(bstFind->key);
		free(bstFind->data);
		free(bstFind);
		return value;
	}
	if ((bstFind->less->data != NULL) && (bstFind->more->data != NULL))
	{
		TreeItem* maxChild = MaxSearch(inputTree, bstFind->less, bstFind->less);
		memcpy(bstFind->key, maxChild->key, sizeof(maxChild->key));
		memcpy(bstFind->data, maxChild->data, tree->sizeofItem);
		BalanceTreeDelete(inputTree, maxChild);
		if (maxChild->parent->less == maxChild)
		{
			maxChild->parent->less = maxChild->less;
			maxChild->less->parent = maxChild->parent;
		}
		if (maxChild->parent->more == maxChild)
		{
			maxChild->parent->more = maxChild->less;
			maxChild->less->parent = maxChild->parent;
		}
		free(maxChild->more->data);
		free(maxChild->more->key);
		free(maxChild->data);
		free(maxChild->key);
		free(maxChild);
		return value;
	}
	return NULL;
}

TreeItem* MaxSearch(Tree* inputTree, TreeItem* inputItem, TreeItem* compareItem)
{
	if ((inputItem != NULL) && (compareItem != NULL))
	{
		if (inputTree->cmpFunc(inputItem->key, compareItem->key) == 3)
		{
			compareItem = inputItem;
		}
		if (inputItem->less->data != NULL)
		{
			compareItem = MaxSearch(inputTree, inputItem->less, compareItem);
		}
		if (inputItem->more->data != NULL)
		{
			compareItem = MaxSearch(inputTree, inputItem->more, compareItem);
		}
	}
	return compareItem;
}

TreeItem* MinSearch(Tree* inputTree, TreeItem* inputItem, TreeItem* compareItem)
{
	if ((inputItem != NULL) && (compareItem != NULL))
	{
		if (inputTree->cmpFunc(inputItem->key, compareItem->key) == 1)
		{
			compareItem = inputItem;
		}
		if (inputItem->less->data != NULL)
		{
			compareItem = MinSearch(inputTree, inputItem->less, compareItem);
		}
		if (inputItem->more->data != NULL)
		{
			compareItem = MinSearch(inputTree, inputItem->more, compareItem);
		}
	}
	return compareItem;
}

TreeItem* FindBrother(TreeItem* inputItem)
{
	if (inputItem->parent->less == inputItem)
	{
		return inputItem->parent->more;
	}
	if (inputItem->parent->more == inputItem)
	{
		return inputItem->parent->less;
	}
	return NULL;
}

void BalanceTreeDelete(Tree* inputTree, TreeItem* inputItem)
{
	TreeItem* item = inputItem;
	while ((item != inputTree->root) && (item->color == 0))
	{
		TreeItem* brother = NULL;
		if (item->parent->less == item)
		{
			brother = item->parent->more;
			if (brother->color == 1)
			{
				brother->color = 0;
				item->parent->color = 1;
				LeftRotate(item->parent);
				brother = item->parent->more;
			}
			if ((brother->less->color == 0) && (brother->more->color == 0))
			{
				brother->color = 1;
				item = item->parent;
			}
			else
			{
				if (brother->more->color == 0)
				{
					brother->less->color = 0;
					brother->color = 1;
					RightRotate(brother);
					brother = item->parent->more;
				}
				brother->color = item->parent->color;
				item->parent->color = 0;
				brother->more->color = 0;
				LeftRotate(item->parent);
				item = inputTree->root;
			}
		}
		else
		{
			brother = item->parent->less;
			if (brother->color == 1)
			{
				brother->color = 0;
				item->parent->color = 1;
				RightRotate(item->parent);
				brother = item->parent->less;
			}
			if ((brother->less->color == 0) && (brother->more->color == 0))
			{
				brother->color = 1;
				item = item->parent;
			}
			else
			{
				if (brother->less->color == 0)
				{
					brother->more->color = 0;
					brother->color = 1;
					LeftRotate(brother);
					brother = item->parent->less;
				}
				brother->color = item->parent->color;
				item->parent->color = 0;
				brother->less->color = 0;
				RightRotate(item->parent);
				item = inputTree->root;
			}
		}
	}
	item->color = 0;
}