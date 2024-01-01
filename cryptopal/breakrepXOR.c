#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stringstuff.h"

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return 0;
	}

	FILE *file = fopen(argv[1], "r");
	fseek(file, 0, SEEK_END);
	labeledstring b64 = {0};
	b64.length = ftell(file);
	fseek(file, 0, SEEK_SET);
	b64.ptr = malloc(b64.length);

	fread(b64.ptr, 1, b64.length, file);
	fclose(file);
	labeledstring bytes = B64ToBytes(b64);

	DoubleArr D = GuessKeySize(bytes, 2, 40);
	char *track = malloc(D.length);
	memset(track, 0, D.length);
	labeledstring guess = {0};
	do
	{
		double MinDistance = D.ptr[0];
		unsigned int MinIndex = 0;
		unsigned int i = 0;
		for(;i < D.length;i++)
			if(!track[i])
			{
				MinDistance = D.ptr[i];
				MinIndex = i;
				break;
			}
		if(i == D.length)
		{
			printf("Out of distances to check. Exiting.\n");
			break;
		}
		for(i = 0;i < D.length;i++)
			if((D.ptr[i] < MinDistance) && !track[i])
			{
				MinDistance = D.ptr[i];
				MinIndex = i;
			}
		track[MinIndex] = 1;
		unsigned int KeySize = MinIndex + 2;
		char *Key = malloc(KeySize + 1);
		labeledstring Transposee = (labeledstring){malloc(bytes.length / KeySize), bytes.length / KeySize};
		for(i = 0; i < KeySize; i++)
		{
			for(unsigned int j = 0; j < bytes.length / KeySize; j++)
				Transposee.ptr[j] = bytes.ptr[i + j * KeySize];
			scoredstring result = CrackSingleByteXOR(Transposee);
			freeScoredList(result.list);
			Key[i] = result.list->item.key;
		}
		guess = XORStrings(bytes, (labeledstring){Key,KeySize});
		free(Key);
		printf("\n\nKeySize: %d\nKey: %.*s\n\n", KeySize, KeySize, Key);
		fwrite(guess.ptr, 1, guess.length, stdout);
	} while(getchar() != EOF);
	printf("Provide filename\n");
	char filename[50];
	scanf("%s", filename);

	FILE *output = fopen(filename, "wb");
	fwrite(guess.ptr, 1, guess.length, output);
	fclose(output);
}
