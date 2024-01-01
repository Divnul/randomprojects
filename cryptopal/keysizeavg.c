#include <stdio.h>
#include <stdlib.h>
#include "stringstuff.h"

unsigned int GuessKeySize(labeledstring bytes, unsigned int MinKeySize, unsigned int MaxKeySize)
{
	unsigned int amount = 0;
	double BestDistance = 0;
	unsigned int BestKeySize = 0;
	for(unsigned int i = MinKeySize; i <= MaxKeySize; i++)
	{
		if(i > bytes.length / 4)
			break;
		amount++;
		double sum = 0;
		double count = 0;
		for(unsigned int j = 0;j < i * 4; j += i)
			for(unsigned int k = j + i; k + i < i * 4; k += i)
			{
				labeledstring A = (labeledstring){bytes.ptr + j, i};
				labeledstring B = (labeledstring){bytes.ptr + k, i};
				sum += HammingDistance(A,B);
				count++;
			}
		double Distance = sum / count / i;
		if(Distance < BestDistance || !BestDistance)
		{
			BestDistance = Distance;
			BestKeySize = i;
		}
	}

	return BestKeySize;
}
int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Provide string.\n");
		return 0;
	}
	labeledstring bytes = HexToBytes(label(argv[1]));
	printf("KeySize: %d", GuessKeySize(bytes, 2, 40));
}
