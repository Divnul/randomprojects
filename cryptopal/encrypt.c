#include <stdio.h>
#include <stdlib.h>
#include "stringstuff.h"

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("Usage: %s <filename> <key> <optional: outputname>\n", argv[0]);
		return 0;
	}
	FILE *file = fopen(argv[1], "rb");
	if(!file)
	{
		perror("Couldn't read file");
		return -1;
	}
	labeledstring buffer = {0};
	fseek(file, 0, SEEK_END);
	buffer.length = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer.ptr = malloc(buffer.length);
	fread(buffer.ptr, 1, buffer.length, file);
	fclose(file);
	labeledstring XOR = XORStrings(buffer, label(argv[2]));
	char *outputfilename;
	if(argc < 4)
	{
		size_t periodpos = 0;
		for(char *ptr = argv[1]; *ptr != 0 && *ptr != '.'; ptr++)
			periodpos++;
		outputfilename = malloc(periodpos + 5);
		memcpy(outputfilename, argv[1], periodpos);
		memcpy(outputfilename + periodpos, ".enc", 5);
	}
	else
		outputfilename = argv[3];

	FILE *outputfile = fopen(outputfilename, "wb");
	fseek(outputfile, 0, SEEK_SET);
	fwrite(XOR.ptr, 1, XOR.length, outputfile);
	fclose(outputfile);
}
