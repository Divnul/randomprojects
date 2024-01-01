#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
	char *ptr;
	size_t length;
} labeledstring;

labeledstring label(char *s)
{
	labeledstring back = {0};
	back.ptr = s;
	for(char *ptr = s; *ptr; ptr++)
		back.length++;
	return back;
}
labeledstring BytesToHex(labeledstring Bytes)
{
	labeledstring output = {0};
	output.ptr = malloc(Bytes.length * 2 + 1);
	output.length = Bytes.length * 2;
	char *hexstr = "0123456789abcdef";
	for(size_t i = 0;i < Bytes.length;i++)
	{
		output.ptr[i*2] = hexstr[Bytes.ptr[i] / 16];
		output.ptr[i*2+1] = hexstr[Bytes.ptr[i] % 16];
	}
	output.ptr[output.length] = 0;
	return output;
}
labeledstring HexToBytes(labeledstring hex)
{
	labeledstring output = {0};
	output.ptr = malloc((hex.length + 1) / 2 + 1);
	output.length = (hex.length + 1) / 2;
	unsigned char sum = 0;
	char cycle = 1;
	size_t i = 0, j = 0;
	for(; i < hex.length; i++)
	{
		sum *= 16;
		sum += hex.ptr[i] > '9' ? hex.ptr[i] % 32 + 9 : hex.ptr[i] - 48;
		if(!cycle)
		{
			output.ptr[j++] = sum;
			cycle = 2;
		}
		cycle--;
	}
	if(!cycle)
	{
		output.ptr[j++] = sum;
	}
	output.ptr[output.length] = 0;
	return output;
}

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("Provide 2 hex strings to XOR together\n");
		return 0;
	}
	labeledstring input = label(argv[1]);
	labeledstring XOR = label(argv[2]);
	printf("%s\n%s\n",input.ptr, XOR.ptr);
	labeledstring output = HexToBytes(input);
	labeledstring readyXOR = HexToBytes(XOR);
	printf("%d\n%d\n", *output.ptr, *readyXOR.ptr);
	for(size_t i = 0;i < output.length, i < readyXOR.length; i++)
	{
		printf("%d ^ %d = ", output.ptr[i], readyXOR.ptr[i]);
		output.ptr[i] ^= readyXOR.ptr[i];
		printf("%d\n", output.ptr[i]);
	}
	output = BytesToHex(output);
	printf("A: %d\n", *output.ptr);
	printf("B: %d\n", *readyXOR.ptr);
	printf("%s", output.ptr);
}
