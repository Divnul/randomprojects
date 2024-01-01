#include <stdio.h>
#include "stringstuff.h"

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Provide hex string to break single char XOR on.\n");
		return 0;
	}
	scoredstring result = CrackSingleByteXOR(HexToBytes(label(argv[1])));
	printf("Result score: %zd\n\n", result.score);
	scoreditem *ptr = result.list;
	while(ptr)
	{
		printf("Result: %s\nKey: %c\n", ptr->item.str.ptr, ptr->item.key);
		ptr = ptr->next;
	}
}
