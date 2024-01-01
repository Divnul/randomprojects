#include <stdio.h>
#include <stdlib.h>
#include "stringstuff.h"

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Usage: %s <HEX rep XOR encoded string> <optional - MaxKeySize> <optional - MinKeySize>\n", argv[0]);
		return 0;
	}
	unsigned int MinKeySize = 2;
	unsigned int MaxKeySize = 40;
	if(argc > 2)
		if(sscanf(argv[2], "%d", &MaxKeySize) != 1)
		{
			printf("Failed to read 2nd argument");
			return -1;
		}
	if(argc > 3)
		if(sscanf(argv[3], "%d", &MinKeySize) != 1)
		{
			printf("Failed to read 2nd argument");
			return -1;
		}
	labeledstring Bytes = HexToBytes(label(argv[1]));
	unsigned int Guess1 = GuessKeySizeAvg(Bytes, MinKeySize, MaxKeySize);
	unsigned int Guess2 = GuessKeySizePair(Bytes, MinKeySize, MaxKeySize);

	printf("Average guess: %d\n Pair guess: %d\n", Guess1, Guess2);
}
