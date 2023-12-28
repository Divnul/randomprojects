#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "Headers/prgtypedefs.h"
#include "Headers/imgfIO.h"

#define BOARDPADDING 20

const BLENDFUNCTION blend = {AC_SRC_OVER,0,255,AC_SRC_ALPHA};
struct
{
	int width;
	int height;
	BITMAPINFO bmi;
	HDC Context;
	HBITMAP Handle;
	pixel *pixels;
	ImageHandler bgImage;
	bool bgImageLoaded;
} frame = {0};
typedef struct
{
	// Pawn, knight, bishop, rook, queen, king
	char x, y, pieceType;
} piece;
struct
{
	char x, y;
} pieceSize = {0};
struct
{
	char x, y;
} HighlightedSquare = {-1,-1};

piece WhitePieces[16];
piece BlackPieces[16];

static ImageHandler BoardSprites = {0};
static ImageHandler board = {0};
static ImageHandler pieceLayer = {0};
static ImageHandler highlightLayer = {0};
static ImageHandler BoardBufferLayer = {0};

static bool quit = false;

ERRORCODE SetUpBoard()
{
	ERRORCODE e = GetImageFile("board",&BoardSprites);
	if(e)
	{
		return e;
	}
	BITMAPINFO bmi = {0};
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = BoardSprites.width / 2;
	bmi.bmiHeader.biHeight = BoardSprites.height * 8 / 9;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	pieceSize.x = BoardSprites.width / 16;
	pieceSize.y = BoardSprites.height / 9;
	board.width = bmi.bmiHeader.biWidth;
	board.height = bmi.bmiHeader.biHeight;
	board.Handle = CreateDIBSection(0, &bmi, DIB_RGB_COLORS, &board.pixels, 0, 0);
	board.Context = CreateCompatibleDC(0);
	SelectObject(board.Context, board.Handle);
	BitBlt(board.Context, 0, 0, board.width, board.height, BoardSprites.Context, 0, 0, SRCCOPY);

	pieceLayer.width = bmi.bmiHeader.biWidth;
	pieceLayer.height = bmi.bmiHeader.biHeight;
	pieceLayer.Handle = CreateDIBSection(0,&bmi, DIB_RGB_COLORS, &pieceLayer.pixels, 0, 0);
	pieceLayer.Context = CreateCompatibleDC(0);
	SelectObject(pieceLayer.Context, pieceLayer.Handle);
	
	highlightLayer.width = bmi.bmiHeader.biWidth;
	highlightLayer.height = bmi.bmiHeader.biHeight;
	highlightLayer.Handle = CreateDIBSection(0,&bmi, DIB_RGB_COLORS, &highlightLayer.pixels, 0, 0);
	highlightLayer.Context = CreateCompatibleDC(0);
	SelectObject(highlightLayer.Context, highlightLayer.Handle);

	BoardBufferLayer.width = bmi.bmiHeader.biWidth;
	BoardBufferLayer.height = bmi.bmiHeader.biHeight;
	BoardBufferLayer.Handle = CreateDIBSection(0,&bmi, DIB_RGB_COLORS, &BoardBufferLayer.pixels, 0, 0);
	BoardBufferLayer.Context = CreateCompatibleDC(0);
	SelectObject(BoardBufferLayer.Context, BoardBufferLayer.Handle);

	char pieces[] = {3, 1, 2, 4, 5, 2, 1, 3};
	
	for(char i = 0;i < 8;i++)
	{
		WhitePieces[i] = (piece){i,0,pieces[i]};
		WhitePieces[i+8] = (piece) {i,1,0};
		BlackPieces[i] = (piece){i,7,pieces[i]};
		BlackPieces[i+8] = (piece){i,6,0};
	}
	return 0;
}
ERRORCODE DrawPieceLayer()
{
	for(int i = 0;i < pieceLayer.width * pieceLayer.height;i++)
		pieceLayer.pixels[i].i = 0;
	for(char i = 0;i < 16;i++)
	{
		if(WhitePieces[i].pieceType != -1)
			AlphaBlend(pieceLayer.Context, WhitePieces[i].x * pieceSize.x, (7 - WhitePieces[i].y) * pieceSize.y, pieceSize.x, pieceSize.y, BoardSprites.Context, pieceSize.x * WhitePieces[i].pieceType, 8 * pieceSize.y, pieceSize.x, pieceSize.y, blend);
		if(BlackPieces[i].pieceType != -1)
			AlphaBlend(pieceLayer.Context, BlackPieces[i].x * pieceSize.x, (7 - BlackPieces[i].y) * pieceSize.y, pieceSize.x, pieceSize.y, BoardSprites.Context, pieceSize.x * (BlackPieces[i].pieceType + 6), 8 * pieceSize.y, pieceSize.x, pieceSize.y, blend);
	}
	BitBlt(BoardBufferLayer.Context, 0, 0, BoardBufferLayer.width, BoardBufferLayer.height, board.Context, 0, 0, SRCCOPY);
	AlphaBlend(BoardBufferLayer.Context, 0, 0, BoardBufferLayer.width, BoardBufferLayer.height, pieceLayer.Context, 0, 0, pieceLayer.width, pieceLayer.height, blend);
	return 0;
}

LRESULT CALLBACK WindowMessageProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS window_class = {0};
	window_class.lpfnWndProc = WindowMessageProc;
	window_class.hInstance = Instance;
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.lpszClassName = "Game Window";
	RegisterClass(&window_class);

	if(SetUpBoard())
		MessageBox(NULL, "Board setup failed.","FATAL ERROR",MB_OK);
	DrawPieceLayer();
	
	frame.bmi.bmiHeader.biSize = sizeof(frame.bmi.bmiHeader);
	frame.bmi.bmiHeader.biPlanes = 1;
	frame.bmi.bmiHeader.biBitCount = 24;
	frame.bmi.bmiHeader.biCompression = BI_RGB;
	frame.bmi.bmiHeader.biWidth = BoardSprites.width / 2;
	frame.bmi.bmiHeader.biHeight = BoardSprites.height * 8 / 9;
	frame.Context = CreateCompatibleDC(0);
	
	ERRORCODE e = GetImageFile("background", &frame.bgImage);
	if(e)
		MessageBox(NULL, "Could not find a background image in application directory. If you want a background, add a background.bmp/background.tga","Alert",MB_OK);
	frame.bgImageLoaded = !e;
	static HWND window_handle;
	window_handle = CreateWindow("Game Window", "Custom Chess", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 640, 300, 640, 480, NULL, NULL, Instance, NULL);

	while(!quit)
	{
		static MSG message = {0};
		while(PeekMessage(&message, window_handle, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&message);
		}
	}
	return 0;
}

void ClearCursorHighlight()
{
	BitBlt(highlightLayer.Context, HighlightedSquare.x * pieceSize.x, (7 - HighlightedSquare.y) * pieceSize.y, pieceSize.x, pieceSize.y, highlightLayer.Context, HighlightedSquare.x * pieceSize.x, (7 - HighlightedSquare.y) * pieceSize.y,SRCINVERT);
	HighlightedSquare.x = -1;
	HighlightedSquare.y = -1;
}
LRESULT CALLBACK WindowMessageProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_QUIT:
		case WM_DESTROY:
		{
			quit = true;
		} break;
		case WM_MOUSELEAVE:
		{
			ClearCursorHighlight();
			RECT invalid = {HighlightedSquare.x * pieceSize.x, HighlightedSquare.y * pieceSize.y, HighlightedSquare.x * pieceSize.x + pieceSize.x, (7 - HighlightedSquare.y) * pieceSize.y + pieceSize.y};
			InvalidateRect(window_handle, &invalid, false);
		} break;
		case WM_MOUSEMOVE:
		{
			POINTS pts = MAKEPOINTS(lParam);
			int min = frame.width < frame.height ? frame.width : frame.height;
			min -= 2 * BOARDPADDING;
			int x = (pts.x - BOARDPADDING) * highlightLayer.width / min;
			int y = (pts.y - BOARDPADDING) * highlightLayer.height / min;
			x /= pieceSize.x;
			y /= pieceSize.y;
			if(HighlightedSquare.x == x && HighlightedSquare.y == y)
				return 0;			
			if(HighlightedSquare.x >= 0 && HighlightedSquare.y >= 0)
			{
				BitBlt(BoardBufferLayer.Context, HighlightedSquare.x * pieceSize.x, HighlightedSquare.y * pieceSize.y, pieceSize.x, pieceSize.y, board.Context, HighlightedSquare.x * pieceSize.x, HighlightedSquare.y * pieceSize.y, SRCCOPY);
				AlphaBlend(BoardBufferLayer.Context, HighlightedSquare.x * pieceSize.x, HighlightedSquare.y * pieceSize.y, pieceSize.x, pieceSize.y, pieceLayer.Context, HighlightedSquare.x * pieceSize.x, HighlightedSquare.y * pieceSize.y, pieceSize.x, pieceSize.y, blend);
			}
			if(pts.x >= BOARDPADDING && pts.y >= BOARDPADDING && x < 8 && y < 8)
			{
				AlphaBlend(BoardBufferLayer.Context, x * pieceSize.x, y * pieceSize.y, pieceSize.x, pieceSize.y, BoardSprites.Context, x * pieceSize.x + board.width, y * pieceSize.y, pieceSize.x, pieceSize.y, blend);
				HighlightedSquare.x = x;
				HighlightedSquare.y = y;
			}
			else
			{
				HighlightedSquare.x = -1;
				HighlightedSquare.y = -1;
			}
			if(frame.bgImageLoaded) StretchBlt(frame.Context, 0, 0, frame.width, frame.height, frame.bgImage.Context, 0, 0, frame.bgImage.width, frame.bgImage.height, SRCCOPY);

			AlphaBlend(frame.Context, BOARDPADDING, BOARDPADDING, min, min, BoardBufferLayer.Context, 0, 0, BoardBufferLayer.width, BoardBufferLayer.height, blend);

			InvalidateRect(window_handle, NULL, false);
			return 0;
		} break;
		case WM_PAINT:
		{
			static PAINTSTRUCT paint;
			static HDC device_context;
			device_context = BeginPaint(window_handle, &paint);

			BitBlt(device_context, 0, 0, frame.width, frame.height, frame.Context, 0, 0, SRCCOPY);

			EndPaint(window_handle, &paint);
		} break;
		case WM_SIZE:
		{
			frame.width = LOWORD(lParam);
			frame.height = HIWORD(lParam);
			frame.bmi.bmiHeader.biWidth = frame.width;
			frame.bmi.bmiHeader.biHeight = frame.height;
			if(frame.Handle) DeleteObject(frame.Handle);
			frame.Handle = CreateDIBSection(NULL, &frame.bmi, DIB_RGB_COLORS, &frame.pixels, 0, 0);
			SelectObject(frame.Context, frame.Handle);

			if(frame.bgImageLoaded) if(!StretchBlt(frame.Context, 0, 0, frame.width, frame.height, frame.bgImage.Context, 0, 0, frame.bgImage.width, frame.bgImage.height, SRCCOPY)) MessageBox(NULL, "Failed to draw background.","Alert",MB_OK);
			
			int min = frame.width < frame.height ? frame.width : frame.height;
			min -= 2 * BOARDPADDING;

			AlphaBlend(frame.Context, BOARDPADDING, BOARDPADDING, min, min, BoardBufferLayer.Context, 0, 0, BoardBufferLayer.width, BoardBufferLayer.height, blend);

			InvalidateRect(window_handle, NULL, false);
		} break;
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
			minMaxInfo->ptMinTrackSize.x = 400;
			minMaxInfo->ptMinTrackSize.y = 400;
		} break;
		default:
		{
			return DefWindowProc(window_handle, message, wParam, lParam);
		}
	}
	return 0;
}

