#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "common.h"

void fillListRandom(int *list, int total, int min, int max)
{
	for(int i = 0; i < total; i++)
	{
		list[i] = (rand() % (max - min + 1)) + min;
	}
}

int *createList(int total)
{
	int *list = (int *) malloc(total *sizeof(int));
	fillList(list, total);
	return list;
}

void fillList(int *list, int total)
{
	for(int i = 0; i < total; i++)
	{
		list[i] = -1;
	}
}
