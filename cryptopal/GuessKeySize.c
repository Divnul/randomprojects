#include <stdio.h>
#include <stdlib.h>
#include "stringstuff.h"

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Usage: %s <hex repkeyXOR> <optional - MaxKeySize> <optional - MinKeySize", argv[0]);
		return 0;
	}
	unsigned int MinKeySize = 2;
	unsigned int MaxKeySize = 40;
	if(argc > 2)
		if(sscanf(argv[2], "%d", &MaxKeySize) != 1)
		{
			printf("Could not read 2nd field\n");
			return -1;
		}
	if(argc > 3)
		if(sscanf(argv[3], "%d", &MinKeySize) != 1)
		{
			printf("Could not read 3rd field\n");
			return -1;
		}

	labeledstring bytes = HexToBytes(label(argv[1]));
	
	if(MinKeySize > bytes.length / 2)
	{
		printf("MinKeySize too big\n");
		return 0;
	}
	unsigned int BestKeySize = MinKeySize;
	double BestDistance = 0;
	double *Distances = malloc((MaxKeySize - MinKeySize + 1) * sizeof(double));
	double FullSum = 0;
	for(unsigned int i = MinKeySize; i <= MaxKeySize; i++)
	{
		if(i > bytes.length / 2)
			break;
		double sum = 0;
		unsigned int count = 0;
		for(unsigned int j = 0; j < bytes.length; j += i)
		{
			for(unsigned int k = j + i; k + i < bytes.length; k += i)
			{
				labeledstring A = (labeledstring){bytes.ptr + j, i};
				labeledstring B = (labeledstring){bytes.ptr + k, i};
				sum += HammingDistance(A,B);
				count++;
			}
		}
		double Distance = sum / count / i;
		FullSum += Distance;
		Distances[i - MinKeySize] = Distance;
		if(BestDistance > Distance || !BestDistance)
		{
			BestDistance = Distance;
			BestKeySize = i;
		}
	}
	unsigned int amount = (bytes.length / 2) > MaxKeySize ? MaxKeySize : (bytes.length / 2);
	amount -= MinKeySize;
	FullSum /= amount;
	printf("Average: %f\n", FullSum);
	for(unsigned int i = 0;i < amount; i++)
	{
		if(Distances[i] < FullSum)
			printf("\t");
		printf("%d key distance: %f\n", i + MinKeySize, Distances[i]);
	}
	printf("The 1st guessed key size is %d\n", BestKeySize);
	BestKeySize = MinKeySize;
	FullSum = 0;
	BestDistance = 0;
	labeledstring A = (labeledstring){bytes.ptr, 0};
	labeledstring B = {0};
	for(unsigned int i = MinKeySize; i <= MaxKeySize; i++)
	{
		if(i > bytes.length / 2)
			break;
		A.length = i;
		B.ptr = A.ptr + i;
		B.length = i;
		double Distance = HammingDistance(A,B) / (double)i;
		FullSum += Distance;
		Distances[i - MinKeySize] = Distance;
		if(Distance < BestDistance || !BestDistance)
		{
			BestDistance = Distance;
			BestKeySize = i;
		}
	}
	FullSum /= amount;
	printf("Average: %f\n", FullSum);
	for(unsigned int i = 0;i < amount; i++)
	{
		if(Distances[i] < FullSum)
			printf("\t");
		printf("%d key distance: %f\n", i + MinKeySize, Distances[i]);
	}
	printf("The 2nd guessed key size is %d\n", BestKeySize);
}
