#include <stdio.h>
#include "stringstuff.h"

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("Please provide 2 strings to XOR together\n");
		return 0;
	}
	labeledstring XOR = XORStrings(label(argv[1]),label(argv[2]));
	printf("%s", BytesToHex(XOR).ptr);
}
