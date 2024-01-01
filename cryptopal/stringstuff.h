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
	for(size_t i = 0; i < Bytes.length;i++)
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
labeledstring B64ToBytes(labeledstring s)
{
	labeledstring l = {0};
	l.ptr = malloc((s.length * 3 + 3) / 4 + 1);
	l.length = (s.length * 3 + 3) / 4;
	char *base64str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	uint32_t sum = 0;
	uint8_t cycle = 3;
	size_t j = 0, i = 0;
	for(; i < s.length; i++)
	{
		sum *= 64;
		uint8_t index = 0;
		for(char *ptr = base64str; *ptr != 0 && *ptr != s.ptr[i]; ptr++)
			index++;
		if(index == 64)
		{
			l.length--;
			continue;
		}
		sum += index;
		if(!cycle)
		{
			l.ptr[j++] = sum / (256 * 256);
			l.ptr[j++] = sum / 256;
			l.ptr[j++] = sum % 256;
			sum = 0;
			cycle = 4;
		}
		cycle--;
	}
	if(cycle != 3)
	{
		if(sum >= (256 * 256))
			l.ptr[j++] = sum / (256 * 256);
		if(sum >= 256)
			l.ptr[j++] = sum / 256;
		l.ptr[j++] = sum % 64;
	}
	l.ptr[j] = 0;
	return l;
}
labeledstring HexTo64(labeledstring s)
{
	labeledstring l = {0};
	l.ptr = malloc((s.length * 2 + 2) / 3 + 1);
	l.length = (s.length * 2 + 2) / 3;
	char *base64str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	uint16_t sum = 0;
	uint8_t cycle = 2;
	size_t j = 0, i = 0;
	for(; i < s.length; i++)
	{
		sum *= 16;
		sum += s.ptr[i] > '9' ? s.ptr[i] % 32 + 9 : s.ptr[i] - 48;
		if(!cycle)
		{
			l.ptr[j++] = base64str[sum / 64];
			l.ptr[j++] = base64str[sum % 64];
			sum = 0;
			cycle = 3;
		}
		cycle--;
	}
	if(cycle != 2)
	{
		if(sum >= 64)
		{
			l.ptr[j++] = base64str[sum / 64];
		}
		l.ptr[j] = base64str[sum % 64];
	}
	l.ptr[l.length] = 0;
	return l;
}
labeledstring XORStrings(labeledstring A, labeledstring B)
{
	labeledstring output = {0};
	output.length = A.length > B.length ? A.length : B.length;
	output.ptr = malloc(output.length + 1);
	for(size_t i = 0;i < output.length;i++)
	{
		output.ptr[i] = A.ptr[i % A.length] ^ B.ptr[i % B.length];
	}
	output.ptr[output.length] = 0;
	return output;
}
size_t ScoreGuess(labeledstring Guess)
{
	size_t score = 0;
	for(size_t i = 0;i < Guess.length;i++)
	{
		if(isalpha(Guess.ptr[i]) || Guess.ptr[i] == ' ')
			score++;
	}
	return score;
}
typedef struct
{
	labeledstring str;
	char key;
} XORString;
typedef struct scoreditem
{
	XORString item;
	struct scoreditem *next;
} scoreditem;
typedef struct
{
	scoreditem *list;
	size_t score;
} scoredstring;
void freeScoredList(scoreditem *list)
{
	if(list->next)
		freeScoredList(list->next);
	free(list);
}
scoredstring CrackSingleByteXOR(labeledstring input)
{
	scoredstring BestGuess = {0};
	size_t maxScore = ScoreGuess(input);
	BestGuess.score = maxScore;
	BestGuess.list = malloc(sizeof(scoreditem));
	BestGuess.list->item = (XORString){input, 0};
	BestGuess.list->next = 0;
	labeledstring XORStr;
	uint16_t Str = 0;
	XORStr.ptr = (char *)&Str;
	XORStr.length = 1;
	char key;
	unsigned int TieCount = 0;
	for(unsigned char i = 1; i; i++)
	{
		Str = i;
		labeledstring attempt = XORStrings(input, XORStr);
		size_t score = ScoreGuess(attempt);
		if(score == maxScore)
		{
			scoreditem *ptr = BestGuess.list;
			while(ptr->next)
				ptr = ptr->next;
			ptr->next = malloc(sizeof(scoreditem));
			ptr->next->item = (XORString){attempt,i};
			ptr->next->next = 0;
		}
		else
		if(score > maxScore)
		{
			if(BestGuess.list)
				freeScoredList(BestGuess.list);
			TieCount = 0;
			key = i;
			BestGuess.list = malloc(sizeof(scoreditem));
			BestGuess.list->item = (XORString){attempt, i};
			BestGuess.list->next = 0;
			BestGuess.score = maxScore;
			maxScore = score;
		}
		else
			free(attempt.ptr);
	}
	return BestGuess;
}
size_t HammingDistance(labeledstring A, labeledstring B)
{
	labeledstring XOR = XORStrings(A, B);
	unsigned char Track[256] = {0};
	size_t sum = 0;
	for(size_t i = 0;i < XOR.length;i++)
	{
		if(!XOR.ptr[i])
			continue;
		if(!Track[XOR.ptr[i]])
		{
			size_t copy = XOR.ptr[i];
			while(copy)
			{
				copy = copy & (copy - 1);
				Track[XOR.ptr[i]]++;
			}
		}
		sum += Track[XOR.ptr[i]];
	}
	free(XOR.ptr);
	return sum;
}
typedef struct
{
	double *ptr;
	unsigned int length;
} DoubleArr;
DoubleArr GuessKeySize(labeledstring bytes, unsigned int MinKeySize, unsigned int MaxKeySize)
{
	double *Distances = malloc((MaxKeySize - MinKeySize + 1) * sizeof(double));
	labeledstring A = (labeledstring){bytes.ptr, 0};
	labeledstring B = {0};
	unsigned int count = 0;
	for(unsigned int i = MinKeySize; i <= MaxKeySize; i++)
	{
		if(i > bytes.length / 2)
			break;
		count++;
		A.length = i;
		B.ptr = A.ptr + i;
		B.length = i;
		double Distance = HammingDistance(A,B) / (double)i;
		Distances[i - MinKeySize] = Distance;
	}
	return (DoubleArr){Distances, count};
}
