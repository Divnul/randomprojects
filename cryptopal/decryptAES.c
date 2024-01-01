#include <stdio.h>
#include <stdlib.h>
#include "stringstuff.h"
#include <openssl/aes.h>

labeledstring decryptAES_ECB(unsigned char *ciphertext, size_t length, char *key)
{
	AES_KEY decryptKey;
	AES_set_decrypt_key(key, 128, &decryptKey);

	unsigned char decryptedText = malloc(length);
	for(size_t i = 0;i < length;i += 16)
		AES_decrypt(ciphertext + i, decryptedText + i, &decryptKey);
	return (labeledstring){decryptedText, length};
}

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("Usage: %s <filename> <AES-128 key>\n");
		printf("Expecting base 64 format in file\n");
		return 0;
	}
	FILE *file = fopen(argv[1]);
	if(!file)
	{
		perror("Couldn't open file\n");
		return -1;
	}

	fseek(file, 0, SEEK_END);
	size_t length = ftell(file);
	labeledstring b64 = (labeledstring){malloc(length), length};
	fseek(file, 0, SEEK_SET);
	fread(b64.ptr, 1, b64.length, file);

	labeledstring bytes = B64ToBytes(b64);
	
	labeledstring Decrypted = decryptAES_ECB(bytes.ptr, bytes.length, argv[2]);
	fwrite(Decrypted.ptr, 1, Decrypted.length, stdout);
}
