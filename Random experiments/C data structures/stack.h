#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CAPACITYINCREMENT 10
typedef struct
{
	void *start;
	unsigned short itemSize;
	unsigned int Top;
	unsigned int Capacity;
} Stack;

Stack InitStack(unsigned short itemSize)
{
	Stack s = {0};
	s.start = malloc(itemSize * CAPACITYINCREMENT);
	s.itemSize = itemSize;
	s.Top = 0;
	s.Capacity = CAPACITYINCREMENT;
	return s;
}
bool StackPush(Stack *stack, void *Item)
{
	if(stack->Top == stack->Capacity)
	{
		void *new = realloc(stack->start, stack->Capacity + CAPACITYINCREMENT * stack->itemSize);
		if(new == 0)
		{
			new = malloc(stack->Capacity + CAPACITYINCREMENT * stack->itemSize);
			if(new == 0)
			{
				//TODO: Handle malloc extreme fatal
				perror("Stack malloc error");
				return false;
			}
			memcpy(new, stack->start, stack->Capacity * stack->itemSize);
			free(stack->start);
		}
		stack->start = new;
	}
	memcpy((char *)stack->start + stack->Top * stack->itemSize, Item, stack->itemSize);
	stack->Top++;
	return true;
}
bool StackPop(Stack *stack, void *copy)
{
	if(!stack->Top)
	{
		// TODO: Handle stack underflow	
		return false;
	}
	stack->Top--;
	if(copy == 0)
		return true;
	memcpy(copy, (char*)stack->start + stack->itemSize * stack->Top,stack->itemSize);
	return true;
}
bool StackPeek(Stack *stack, void *copy)
{
	if(!stack->Top)
	{
		// TODO: Handle stack underflow
		return false;
	}
	memcpy(copy, (char*)stack->start + stack->itemSize * (stack->Top - 1),stack->itemSize);
	return true;
}
