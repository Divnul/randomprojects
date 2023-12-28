#include <stdio.h>
#include <stdlib.h>
#include <mystrings.h>
#pragma pack(push, 1)
typedef struct {
	unsigned short bfType;
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;
} BITMAPFILEHEADER;
typedef struct
{
	unsigned int biSize;
	int biWidth;
	int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
} BITMAPINFOHEADER;
typedef struct
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} PIXEL;
int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Usage: %s <square_pixel_width>\n", argv[0]);
		printf("Result: Create a board.bmp file with specific square size. Ex 16 = 16x16 px");
		return 1;
	}
	bool success = true;
	int pixels = StrToIntA(argv[1], &success);
	int frstQuarter = pixels / 8;
	int lstQuarter = pixels - frstQuarter -1;
	int pieceLstWidth = pixels * 12;
	if(!success)
	{
		printf("%s is not a valid integer parameter. Use command %s for help", argv[1], argv[0]);
		return 2;
	}
	if(pixels < 8)
	{
		printf("Pixel count too small. Please input an integer value >=8");
		return 3;
	}
	FILE *file = fopen("template.bmp", "wb");
	if(file == NULL)
	{
		printf("Error: could not open file 'template.bmp'");
		return 4;
	}
	int width = pixels * 16;
	int height = pixels * 9;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader = {0};

	infoHeader.biSizeImage = width * height * sizeof(PIXEL);
	PIXEL *Pixels = malloc(infoHeader.biSizeImage);

	fileHeader.bfType = 0x4D42;
	fileHeader.bfSize = sizeof(fileHeader) + sizeof(infoHeader) + infoHeader.biSizeImage;
	fileHeader.bfOffBits = sizeof(fileHeader) + sizeof(infoHeader);
	
	infoHeader.biSize = sizeof(infoHeader);
	infoHeader.biWidth = width;
	infoHeader.biHeight = height;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 24;

	for(int i = 0;i < width * height;i++)
	{
		Pixels[i] = (PIXEL){0,255,0};
	}

	for(int i = 0;i < frstQuarter;i++)
	{
		for(int j = 0;j < pieceLstWidth;j++)
		{
			Pixels[i * width + j].green = 0;
		}
	}
	for(int i = frstQuarter;i <= lstQuarter;i++)
	{
		for(int j = 0;j < pieceLstWidth;j++)
		{
			int mod = j % pixels;
			if(mod < frstQuarter || mod > lstQuarter)
				Pixels[i * width + j].green = 0;
		}
	}
	for(int i = lstQuarter+1;i < pixels;i++)
	{
		for(int j = 0;j < pieceLstWidth;j++)
		{
			Pixels[i * width + j].green = 0;
		}
	}
	for(int i = pixels;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			int pixel = i * width + j;
			int x = j / pixels;
			int y = i / pixels;
			y--;
			char square = (x + y) % 2;
			if(x < 8)
			{
				if(square == 0)
				{
					Pixels[pixel].green = 0;
				} else
				{
					Pixels[pixel].blue = 255;
					Pixels[pixel].red = 255;
				}
			}
			else
			{
				if(square == 0)
				{
					Pixels[pixel].green = 0;
					Pixels[pixel].blue = 125;
				}
				else
				{
					Pixels[pixel].red = 200;
					Pixels[pixel].green = 200;
					Pixels[pixel].blue = 255;
				}
			}
		}
	}
	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(Pixels, sizeof(PIXEL), width * height, file);
	fclose(file);
}
