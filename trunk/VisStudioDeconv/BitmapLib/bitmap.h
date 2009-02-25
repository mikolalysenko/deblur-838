#ifndef BITMAP_H
#define BITMAP_H

#include <stdio.h>

//#ifndef _WINDOWS
#ifndef WIN32

typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef long				LONG;

struct BITMAPFILEHEADER
{
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
};

struct BITMAPINFOHEADER
{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
};

#else 

#include <windows.h>

#endif

inline int padding( const int size, const int alignment )
{
	int pad = size % alignment;
	return ( pad == 0 ) ? 0 : ( alignment - pad );
}

bool LoadBMP(const char *pathname, unsigned char *&pixels, 
			 int &width, int &height, int &channels, const int alignment, const bool top2bot );
bool SaveBMP(const char *pathname, const unsigned char *pixels, 
			 const int width, const int height, const int channels, const int alignment, const bool top2bot );
#endif
