#include <stdio.h>
#include "prgtypedefs.h"
#include <windows.h>

#pragma once

typedef unsigned char ERRORCODE; 
#define READFILE(ptr,size,elem,file)			\
	do							\
	{							\
		size_t amount = fread(ptr, size, elem, file);	\
		if(amount != elem)				\
			return 2;				\
	} while(0)

ERRORCODE OpenBMPFile(char *, ImageHandler *);
ERRORCODE OpenTGAFile(char *, ImageHandler *);
ERRORCODE GetImageFile(char *path, ImageHandler *image)
{
	int length = 0;
	char *iterate = path;
	while(*iterate != '\0')
	{
		length++;
		iterate++;
	}
	char *input = malloc(length + 5);
	memcpy(input,path,length);
	iterate = input + length;
	*iterate++ = '.';
	*iterate++ = 'b';
	*iterate++ = 'm';
	*iterate++ = 'p';
	*iterate = '\0';
	ERRORCODE e = OpenBMPFile(input, image);
	if(e == 0)
		return 0;
	if(e > 1)
		return e;
	iterate = input + length;
	*iterate++ = '.';
	*iterate++ = 't';
	*iterate++ = 'g';
	*iterate++ = 'a';
	e = OpenTGAFile(input, image);
	return e;
}

ERRORCODE OpenBMPFile(char *path, ImageHandler *image)
{
	FILE *file = fopen(path, "rb");
	if(!file || !image)
		return 1;
	BITMAPFILEHEADER fileHeader;
	READFILE(&fileHeader, sizeof(fileHeader), 1, file);
	BITMAPINFO bmi;
	READFILE(&bmi.bmiHeader, sizeof(bmi.bmiHeader), 1, file);
	if(bmi.bmiHeader.biSize != 40)
		return 2;
	if(fseek(file, fileHeader.bfOffBits, SEEK_SET))
		return 2;
	image->width = bmi.bmiHeader.biWidth;
	image->height = bmi.bmiHeader.biHeight;
	int biBitCount = bmi.bmiHeader.biBitCount;
	bmi.bmiHeader.biBitCount = 32;
	image->Handle = CreateDIBSection(0,&bmi,DIB_RGB_COLORS,&image->pixels,0,0);
	if(!image->Handle)
		return 3;
	switch(biBitCount)
	{
		case 24:
		{
			int padding = bmi.bmiHeader.biWidth * 3;
			padding = padding % 4;
			padding = 4 - padding;
			padding = padding % 4;
			for(int j = 0;j < bmi.bmiHeader.biHeight;j++)
			{
				for(int i = 0;i < bmi.bmiHeader.biWidth;i++)
				{
					int current = j * bmi.bmiHeader.biWidth + i;
					READFILE(image->pixels+current, 3, 1, file);
					image->pixels[current].channels.alpha = 0;
					if(image->pixels[current].i != 255 << 8)
						image->pixels[current].channels.alpha = 255;
				}
				fseek(file, padding, SEEK_CUR);
			}
		} break;
		case 32:
		{

			READFILE(image->pixels,4,bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight,file);
		} break;
		default:
			return 2;
	}
	image->Context = CreateCompatibleDC(0);
	if(!image->Context)
		return 3;
	SelectObject(image->Context, image->Handle);
	fclose(file);
	return 0;
}
// TGA file handling:

typedef unsigned short dbyte;
#pragma pack(push,1)
typedef struct
{
	BYTE ImageIDLength; // Length of Image ID virtual data field used by developers after TGAFILEHEADER
	BYTE ColorMapType; //0: no color map. 1: Present. 2-127: reserved. 128-255:Developer's choice.
	BYTE ImageType; // 4th bit: RLE present. 0: no image data present. 1: uncompressed color-mapped image. 2: uncompressed true-color image. 3: uncompressed black-and-white (grayscale) image. +2^3 for compression
	struct
	{
		dbyte MapStart; //Index of first color map entry that is included in the file.
		dbyte Length; //Number of entries of the color map that are included in the file.
		BYTE EntrySize; //Number of bits per color map entry.
	} ColorMapSpec;
	struct
	{
		dbyte xOrigin;
		dbyte yOrigin; //These 2 fields represent the pixel coordinate in the image data that acts as the bottom left corner while actually displaying the image. Typically 0,0 or 0,ImageHeight to flip the image.
		dbyte width;
		dbyte height;
		BYTE pixelDepth; //Bits per pixel
		BYTE ImgDesc; //Can be ignored for now.
	} ImageSpec;
} TGAFILEHEADER;

ERRORCODE OpenTGAFile(char* path, ImageHandler *image)
{
	FILE *file = fopen(path, "rb");
	if(!file || !image)
		return 1;
	TGAFILEHEADER fileHeader;
	READFILE(&fileHeader, sizeof(fileHeader), 1, file);
	if(fseek(file, fileHeader.ImageIDLength,SEEK_CUR))
		return 2;
	if(fileHeader.ImageType != 2)
		return 2;
	BITMAPINFO bmi = {0};
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = fileHeader.ImageSpec.width;
	bmi.bmiHeader.biHeight = fileHeader.ImageSpec.height;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biCompression = BI_RGB;

	image->Handle = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &image->pixels, 0, 0);
	image->width = fileHeader.ImageSpec.width;
	image->height = fileHeader.ImageSpec.height;
	image->Context = CreateCompatibleDC(0);
	if(!image->Context || !image->Handle)
		return 3;
	SelectObject(image->Context, image->Handle);

	switch(fileHeader.ImageSpec.pixelDepth)
	{
		case 24:
		{
			for(int i = 0;i < image->width * image->height;i++)
			{
				READFILE(&image->pixels + i,3,1,file);
				image->pixels[i].channels.alpha = 0;
				if(image->pixels[i].i != 255 << 8)
				       image->pixels[i].channels.alpha = 255;	
			}
		} break;
		case 32:
		{
			READFILE(image->pixels,4,fileHeader.ImageSpec.width * fileHeader.ImageSpec.height,file);
		} break;
		default:
			return 2;
	}
	fclose(file);
	return 0;
}
