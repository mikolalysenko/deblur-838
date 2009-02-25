#ifndef PIXMAP_H
#define PIXMAP_H

#include <stdio.h>

inline void read_comment(FILE *fp)
{
	if (fgetc(fp) == '#')
	{
		while (fgetc(fp) != '\n');
		read_comment(fp);
	}
	else 
	{
		fseek(fp, -1, SEEK_CUR);
	}
}
	
struct PixMapHeader
{
	unsigned int _width, _height, _maxval;
};

void SwapBytes(unsigned char* pixels, int nPixels, int B);

bool LoadPGM(const char *pathname, unsigned char *&pixels, 
			 int &width, int &height, int &maxval);

bool SavePGM(const char *pathname, const unsigned char *pixels, 
			 const int width, const int height, const int maxval);

bool LoadPPM(const char *pathname, unsigned char *&pixels, 
			 int &width, int &height, int &maxval);

bool SavePPM(const char *pathname, const unsigned char *pixels, 
			 const int width, const int height, const int maxval);

bool LoadPFM(const char *pathname, float *&pixels, 
			 int &width, int &height, float &maxval, int &nbands);

bool SavePFM(const char *pathname, const float *pixels, 
			 const int width, const int height, const float maxval, const int nbands);

#endif
