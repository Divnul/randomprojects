#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "stringstuff.h"

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("Please provide 2 strings to calculate the hamming distance of.\n");
		return 0;
	}
	size_t dist = HammingDistance(label(argv[1]),label(argv[2]));
	printf("Hamming distance is %zd bits", dist);
}
