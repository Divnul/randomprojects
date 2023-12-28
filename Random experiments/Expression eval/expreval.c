#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../CDataStructures/Stack.h"

// STARTREGION: Possible character types
#define isNumber(c) ((c) >= '0' && (c) <= '9')
#define isAlphabetChar(c) ((c) >= 'a' && (c) <= 'z')
#define isOperator(c) ((c) == '+' || (c) == '-' || (c) == '*' || (c) == '/' || (c) == '(' || (c) == ')')
// ENDREGION

char *getLine(char *question, unsigned char bufferSize)
{
	char *buffer = malloc(bufferSize);
	while(1)
	{
		printf(question);
		if(!fgets(buffer, bufferSize, stdin))
		{
			perror("Failed to fetch line input");
			printf("Please try again\n");
			continue;
		}
		char *ptr = buffer;
		bool newLine = false;
		while(*ptr != 0)
			if(*ptr++ == '\n')
			{
				newLine = true;
				break;
			}
		if(!newLine)
		{
			printf("Input too big for buffer. Please limit input to %d characters.\n", bufferSize - 2);
			continue;
		}
		return buffer;
	}
}
double getDouble(char *question, unsigned char bufferSize)
{
	while(1)
	{
		char *answer = getLine(question, bufferSize);
		char *endptr;
		double value = strtod(answer, &endptr);
		if(*endptr != 0 && *endptr != '\n')
		{
			free(answer);
			printf("Invalid double.\n");
			continue;
		}
		return value;	
	}
}
int main(int argc, char *argv[])
{
	if(argc <= 1)
	{
		printf("Please provide %s with a valid numerical expression to evaluate.",argv[0]);
		return 0;
	}
	int length = 0;
	for(int arg = 1; arg < argc; arg++)
	{
		char *ptr = argv[arg];
		while(*ptr++ != 0)
			length++;
	}
	char *input = (char *)malloc(length + 1);
	int iterate = 0;
	for(int arg = 1; arg < argc; arg++)
	{
		char *ptr = argv[arg];
		while(*ptr != 0)
			*(input + iterate++) = *ptr++;
	}
	*(input + iterate) = 0;
	unsigned long Bitmap = 0;
	double Variables[64];
	for(char *ptr = input, c = *input; c != 0; c = *ptr++)
	{
		if(c == ' ' || c == '\n' || c == '.')
			continue;
		if(isAlphabetChar(c))
		{
			unsigned int bmp = 1 << (c % 64);
			if(!(Bitmap & bmp))
			{
				char varRequest[51];
				sprintf(varRequest, "Unknown variable %c found. Please enter its value:\n", c);
				double value = getDouble(varRequest, 10);
				Variables[c % 64] = value;
				Bitmap |= bmp;
			}
			continue;
		}
		if(!isOperator(c) && !isNumber(c))
		{
			printf("Unexpected character found at %d\n",(int)(ptr - input));
			return -1;
		}
	}
	Stack operands = InitStack(sizeof(double));
	Stack operators = InitStack(1);

	for(char *ptr = input, c = *input; c != 0; c = *(++ptr))
	{
	
		// Skip any whitespace
		if(c == ' ')
			continue;

		// Check the possibilities that this char represents an operand.
		if(isAlphabetChar(c))
		{
			StackPush(&operands, Variables + (c % 64));
			continue;
		}
		if(isNumber(c))
		{
			double fetch = strtod(ptr, &ptr);
			StackPush(&operands, &fetch);
			ptr--;
			continue;
		}
		
		// Now we know it isn't an operand (literal / variable)
		// It could still potentially be a '.'
		if(c == '.')
		{
			printf("Unexpected character found at %d\n", (int)(ptr - input));
			return -1;
		}
		// Guaranteed to be an operator in normal execution order
		if(!operators.Top)
		{
			StackPush(&operators, &c);
			continue;
		}
		if(c == ')')
		{
			while(1)
			{
				char operator;
				StackPop(&operators, &operator);
				if(operator == '(')
					break;
				double b = 0;
				double a = 0;
				if(!StackPop(&operands, &b) || !StackPop(&operands, &a))
				{
					printf("Error: operator lacks operand(s)\n");
					return -1;
				}
				switch(operator)
				{
					case '+':
						a += b;
						break;
					case '-':
						a -= b;
						break;
					case '*':
						a *= b;
						break;
					case '/':
						a /= b;
						break;
				}
				StackPush(&operands, &a);
				if(!operators.Top)
				{
					printf("Unexpected character ')' at %d\n", (int)(ptr - input));
					return -1;
				}
			}
			continue;
		}
		char previous;
		StackPeek(&operators, &previous);
		if(c == '(' || previous == '(')
		{
			StackPush(&operators, &c);
			continue;
		}
		char curPrecedence;
		char prevPrecedence;
		if(c == '+' || c == '-')
			curPrecedence = 1;
		else
			curPrecedence = 2;
		if(previous == '+' || previous == '-')
			prevPrecedence = 1;
		else
			prevPrecedence = 2;
		if(prevPrecedence < curPrecedence)
		{
			StackPush(&operators, &c);
			continue;
		}
		while(curPrecedence <= prevPrecedence)
		{
			char operator;
			StackPop(&operators, &operator);
			
			double b = 0;
			double a = 0;
			if(!StackPop(&operands, &b) || !StackPop(&operands, &a))
			{
				printf("Error: operator lacks operand(s)\n");
				return -1;
			}
			switch(operator)
			{
				case '+':
					a += b;
					break;
				case '-':
					a -= b;
					break;
				case '*':
					a *= b;
					break;
				case '/':
					a /= b;
					break;
			}
			StackPush(&operands, &a);
			if(!StackPeek(&operators, &operator))
			{
				StackPush(&operators, &c);
				break;
			}
			if(operator == '(')
			{
				StackPush(&operators, &c);
				break;
			}
			if(operator == '+' || operator == '-')
				prevPrecedence = 1;
			else
				prevPrecedence = 2;
		}
	}
	while(operators.Top)
	{
		char operator;
		StackPop(&operators, &operator);

		double b = 0;
		double a = 0;
		if(!StackPop(&operands, &b) || !StackPop(&operands, &a))
		{
			printf("Error: operator lacks operand(s)\n");
			return -1;
		}
		switch(operator)
		{
			case '+':
				a += b;
				break;
			case '-':
				a -= b;
				break;
			case '*':
				a *= b;
				break;
			case '/':
				a /= b;
				break;
		}
		StackPush(&operands, &a);
	}
	double output = 0;
	StackPop(&operands, &output);
	printf("Output:\n%lf", output);
}
