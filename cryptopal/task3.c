#include <stdio.h>
#include <stdlib.h>
#include "stringstuff.h"
#include <ctype.h>

labeledstring XORStrings(labeledstring A, labeledstring B)
{
	labeledstring output = {0};
	output.length = A.length > B.length ? A.length : B.length;
	output.ptr = malloc(output.length + 1);
	for(size_t i = 0; i < output.length; i++)
	{
		output.ptr[i] = A.ptr[i % A.length] ^ B.ptr[i % B.length];
	}
	output.ptr[output.length] = 0;
	return output;
}
size_t ScoreGuess(labeledstring Guess)
{
	size_t score = 0;
	for(size_t i = 0;i < Guess.length;i++)
	{
		if(isalpha(Guess.ptr[i]) || Guess.ptr[i] == ' ')
			score++;
	}
	return score;
}
labeledstring CrackSingleByteXOR(labeledstring input)
{
	labeledstring BestGuess = {0};
	size_t maxScore = ScoreGuess(input);
	labeledstring XORStr;
	uint16_t Str = 0;
	XORStr.ptr = (char *)&Str;
	XORStr.length = 1;
	for(unsigned char i = 1; i; i++)
	{
		Str = i;
		labeledstring try = XORStrings(input, XORStr);
		size_t score = ScoreGuess(try);
		if(score > maxScore)
		{
			if(BestGuess.ptr)
				free(BestGuess.ptr);
			BestGuess = try;
			maxScore = score;
		}
		else
			free(try.ptr);
	}
	return BestGuess;
}


int main(int argc, char **argv)
{
	if(argc == 1)
	{
		printf("Provide XOR cipher hex to crack");
		return 0;
	}
	printf("%s",CrackSingleByteXOR(HexToBytes(label(argv[1]))).ptr);
}
