#pragma once
#include <stdio.h>
#include <stdbool.h>

int StrToIntA(char *in, bool *success)
{
	char *Iterator = in;
	while(*Iterator == ' ')
		Iterator++;
	int out = 0;
	char sign = 1;
	if(*Iterator == '-')
	{
		sign = -1;
		Iterator++;
		while(*Iterator == ' ')
			Iterator++;
	}
	while(*Iterator != '\0')
	{
		if(*Iterator == ' ')
		{
			while(*(++Iterator) != '\0')
				if(*Iterator != ' ')
				{
					*success = false;
					return 0;
					//Handle invalid string error: Several words in string
				}	
			break;
		}
		out *= 10;
		if(*Iterator < 48 || *Iterator > 57)
		{
			*success = false;
			return 0;
			//Handle invalid integer string error: non numerical characters
		}
		out += sign * (*Iterator - 48);
		Iterator++;
	}
	return out;
}