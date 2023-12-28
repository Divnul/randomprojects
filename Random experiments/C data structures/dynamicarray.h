// CDataStructures/dynamicarray.h

#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define DEFCAPINCREMENT 10
typedef struct
{
	void *start;
	unsigned int length;
	unsigned int capacity;
	unsigned short itemSize;
	unsigned short capacityIncrement;
} DynamicArray;

DynamicArray InitDynamicArray(unsigned short itemSize, unsigned short capacityIncrement)
{
	DynamicArray DA = {0};
	DA.capacity = capacityIncrement ? capacityIncrement : DEFCAPINCREMENT;
	DA.capacityIncrement = DA.capacity;
	DA.start = malloc(itemSize * DA.capacity);
	DA.itemSize = itemSize;
	return DA;
}

bool DAAddItem(DynamicArray *DA, void *Item)
{
	if(DA->length == DA->capacity)
	{
		void *newStart = realloc(DA->start, (DA->capacity + DA->capacityIncrement) * DA->itemSize);
		if(newStart == NULL)
		{
			newStart = malloc((DA->capacity + DA->capacityIncrement) * DA->itemSize);
			if(newStart == NULL)
			{
				perror("Dynamic array alloc failed");
				return false;
			}
			memcpy(newStart, DA->start, DA->capacity * DA->itemSize);
			free(DA->start);
		}
		DA->start = newStart;
		DA->capacity = DA->capacity + DA->capacityIncrement;
	}
	memcpy((char *)DA->start + DA->length * DA->itemSize, Item, DA->itemSize);
	DA->length++;
	return true;
}
bool DAAddItems(DynamicArray *DA, void *ItemArr, unsigned int ItemCount)
{
	unsigned int newCapacity = DA->length + ItemCount + DA->capacity - 1;
	newCapacity -= newCapacity % DA->capacity;
	if(newCapacity > DA->capacity)
	{
		void *newStart = realloc(DA->start, newCapacity * DA->itemSize);
		if(newStart == NULL)
		{
			newStart = malloc(newCapacity * DA->itemSize);
			if(newStart == NULL)
				return false;
			memcpy(newStart, DA->start, DA->length * DA->itemSize);
			free(DA->start);
		}
		DA->start = newStart;
		DA->capacity = newCapacity;
	}
	memcpy((char *)DA->start + DA->length * DA->itemSize, ItemArr, ItemCount * DA->itemSize);
	DA->length = DA->length + ItemCount;
	return true;
}
		
bool DARemoveItem(DynamicArray *DA, unsigned int Position)
{
	if(Position >= DA->length)
	{
		fprintf(stderr, "Error: indexing element outside of array");
		return false;
	}
	memcpy((char *)DA->start + Position * DA->itemSize, (char *)DA->start + (Position + 1) * DA->itemSize, (DA->length - Position - 1) * DA->itemSize);
	DA->length--;
	return true;
}
bool DAIndex(DynamicArray *DA, unsigned int Position, void *out)
{
	if(Position >= DA->length)
		return false;
	memcpy(out, (char *)DA->start + Position * DA->itemSize, DA->itemSize);
	return true;
}
bool DASetPos(DynamicArray *DA, unsigned int Position, void *in)
{
	if(Position >= DA->length)
	{
		fprintf(stderr, "Error: indexing element outside of array");
		return false;
	}
	memcpy((char *)DA->start + Position * DA->itemSize, in, DA->itemSize);
	return true;
}
