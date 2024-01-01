#include <stdio.h>
#include <stdlib.h>
#include "stringstuff.h"

unsigned int GuessKeySize(labeledstring bytes, unsigned int MinKeySize, unsigned int MaxKeySize)
{
	double *Distances = malloc((MaxKeySize - MinKeySize + 1) * sizeof(double));
	double FullSum = 0;

	unsigned int count = 0;
	for(unsigned int i = MinKeySize; i <= MaxKeySize; i++)
	{
		if(i > bytes.length / 2)
			break;
		count++;
		double sum = 0;
		double amount = 0;
		for(unsigned int j = 0;j < bytes.length; j+= i)
			for(unsigned int k = j + i; k + i < bytes.length; k+= i)
			{
				labeledstring A = (labeledstring){bytes.ptr + j, i};
				labeledstring B = (labeledstring){bytes.ptr + k, i};
				sum += HammingDistance(A,B);
				amount++;
			}
		double Distance = sum / amount / i;
		FullSum += Distance;
		Distances[i - MinKeySize] = Distance;
	}
	FullSum /= count;
	char *included = malloc(count);
	for(unsigned int i = MinKeySize; i <= MaxKeySize; i++)
		included[i - MinKeySize] = Distances[i - MinKeySize] < FullSum;
	unsigned char *counts = malloc(count);
	memset(counts, 0, count);
	for(unsigned int i = 0;i < count; i++)
	{
		if(!included[i])
			continue;
		for(unsigned int j = i + MinKeySize; j <= MaxKeySize; j++)
			if(included[j - MinKeySize] && ((i + MinKeySize) % j == 0))
				counts[j - MinKeySize]++;
	}
	unsigned char MaxCount = 0;
	unsigned char MaxIndex = 0;
	for(unsigned int i = 0;i < count;i++)
		if(counts[i] > MaxCount)
		{
			MaxCount = counts[i];
			MaxIndex = i;
		}
	return MaxIndex + MinKeySize;
}
int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Please provide a hex string to guess the key size of\n");
		return 0;
	}
	labeledstring bytes = HexToBytes(label(argv[1]));
	unsigned int KeySize = GuessKeySize(bytes, 2, 40);
	printf("Key Size: %d\n", KeySize);
}
