#include <stdint.h>
#pragma once

typedef union
{
	uint32_t i;
	struct
	{
		BYTE blue;
		BYTE green;
		BYTE red;
		BYTE alpha;
	} channels;
} pixel;
typedef struct
{
	int width;
	int height;
	HBITMAP Handle;
	HDC Context;
	pixel *pixels;
} ImageHandler;
