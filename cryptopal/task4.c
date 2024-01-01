#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stringstuff.h"

int main(int argc, char **argv)
{
	if(argc == 1)
	{
		printf("Please provide filename");
		return 0;
	}
	FILE *file = fopen(argv[1],"r");
	if(!file)
	{
		perror("Couldn't open file");
		return -1;
	}
	char currentstr[61];
	scoredstring bestGuess = {0};
	labeledstring current = {0};
	current.ptr = currentstr;
	current.ptr[60] = 0;
	current.length = 60;
	int character = 0;
	char c;
	while((c = fgetc(file)) != EOF)
	{
		if(c == '\n')
		{
			if(character == 60)
			{
				labeledstring bytes = HexToBytes(current);
				scoredstring guess = CrackSingleByteXOR(bytes);
				if(guess.score > bestGuess.score)
				{
					if(bestGuess.str.ptr)
						free(bestGuess.str.ptr);
					bestGuess = guess;
				}
				else
					free(guess.str.ptr);
				free(bytes.ptr);
			}
			character = 0;
			continue;
		}
		if(character >= 60)
			continue;
		current.ptr[character++] = c;
	}
	if(character == 60)
	{
		labeledstring bytes = HexToBytes(current);
		scoredstring guess = CrackSingleByteXOR(bytes);
		if(guess.score > bestGuess.score)
			bestGuess = guess;
	}
	printf("%s\n", bestGuess.str.ptr);
	printf("%zd",bestGuess.score);
	fclose(file);
}
