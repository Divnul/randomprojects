#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stringstuff.h"

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Type hex for testing");
		return 0;
	}
	labeledstring hex = label(argv[1]);

	labeledstring b64 = HexTo64(hex);
	printf("64: %s\n", b64.ptr);

	labeledstring ToBytes = B64ToBytes(b64);
	printf("Bytes: %s\n", ToBytes.ptr);
	labeledstring ToHex = BytesToHex(ToBytes);
	printf("Hex: %s\n", ToHex.ptr);
}
