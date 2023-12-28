#include <stdio.h>
#include <limits.h>
#include "../CDataStructures/stack.h"

int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		printf("Enter %s <brainfuck code>",argv[0]);
		return -1;
	}
	unsigned char cells[100] = {0};
	int pointer = 50;
	Stack LoopStarts = InitStack(1);
	for(int i = 0;argv[1][i] != '\0';i++)
	{
		switch(argv[1][i])
		{
			case '>':
				if(pointer == sizeof(cells))
				{
					printf("ERROR: Pointer overflow.\n");
					return -1;
				}
				pointer++;
				break;
			case '<':
				if(pointer == 0)
				{
					printf("ERROR: Pointer underflow.\n");
					return -1;
				}
				pointer--;
				break;
			case '+':
				cells[pointer]++;
				break;
			case '-':
				cells[pointer]--;
				break;
			case '.':
				printf("%c",cells[pointer]);
				break;
			case ',':
				cells[pointer] = getchar();
				break;
			case '[':
				if(!cells[pointer])
				{
					int scope = 0;
					for(i++;argv[1][i] != '\0';i++)
					{
						if(argv[1][i] == '[')
						{
							scope++;
						}
						else if(argv[1][i] == ']')
						{
							if(!scope)
							{
								break;
							}
							else
							{
								scope--;
							}
						}
					}
					if(argv[1][i] == '\0')
					{
						printf("ERROR: Lonely loop entry at %d.\n",i);
						return -1;
					}
				}
				else
				{
					StackPush(&LoopStarts, &i);
				}
				break;
			case ']':
				if(!LoopStarts.Top)
				{
					printf("ERROR: Lonely loop end at %d.\n",i);
					return -1;
				}
				if(cells[pointer])
				{
					StackPeek(&LoopStarts,&i);
				} else
				{
					StackPop(&LoopStarts,0);
				}
				break;
			case 'p':
				printf("%d",cells[pointer]);
				break;
		}
	}
}
