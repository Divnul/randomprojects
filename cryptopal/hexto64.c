#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
	char *ptr;
	size_t length;
} labeledstring;
labeledstring hexto64(labeledstring s)
{
	labeledstring l = {0};
	l.ptr = malloc((s.length * 2 + 2) / 3 + 1);
	l.length = (s.length * 2 + 2) / 3;
	char *base64alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	uint16_t sum = 0;
	unsigned char cycle = 2;
	size_t j = 0, i = 0;
	for(; i < s.length; i++)
	{
		sum *= 16;
		sum += s.ptr[i] > '9' ? s.ptr[i] % 32 + 9 : s.ptr[i] - 48;
		if(!cycle)
		{
			l.ptr[j++] = base64alph[sum / 64];
			l.ptr[j++] = base64alph[sum % 64];
			sum = 0;
			cycle = 3;
		}
		cycle--;
	}
	if(cycle != 2)
	{
		if(sum >= 64)
		{
			l.ptr[j++] = base64alph[sum / 64];
		}
		l.ptr[j] = base64alph[sum % 64];
	}
	return l;
}

int main(int argc, char **argv)
{
	size_t length = 0;
	if(argc == 1)
	{
		printf("Provide a hex string to turn into base 64\n");
		return 0;
	}
	for(char *ptr = argv[1]; *ptr != 0;ptr++)
		length++;
	labeledstring l = {0};
	l.ptr = argv[1];
	l.length = length;

	labeledstring output = hexto64(l);
	output.ptr[output.length] = 0;
	printf("%s",output.ptr);
}
