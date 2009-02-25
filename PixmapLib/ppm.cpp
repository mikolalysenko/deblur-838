#include <string.h>

#include "pixmap.h"

bool LoadPPM(const char *pathname, unsigned char *&pixels, 
			 int &width, int &height, int &maxval)
{
	FILE *fp;
	if ( ( fp = fopen( pathname, "rb" ) ) == NULL )
	{
		printf( "fail to open file %s!\n", pathname );
		return false;
	}	

	//read header

	read_comment(fp);	
	char magic[16];
	fscanf(fp, "%s", magic);
	if (strcmp(magic, "P6")!=0)
	{
		printf("LoadPPM : invalid magic number in %s!\n", pathname);
		return false;
	}
	fgetc(fp);

	read_comment(fp);
	fscanf(fp, "%d", &width);
	if (width <= 0)
	{
		printf("LoadPPM : invalid width in %s!\n", pathname);
		return false;
	}
	fgetc(fp);

	read_comment(fp);
	fscanf(fp, "%d", &height);
	if (height <= 0)
	{
		printf("LoadPPM : invalid height in %s!\n", pathname);
		return false;
	}
	fgetc(fp);

	read_comment(fp);
	fscanf(fp, "%d", &maxval);
	if (maxval < 1 || maxval > 65535)
	{
		printf("LoadPPM : invalid maxval in %s\n", pathname);
		return false;
	}
	fgetc(fp);

	//read pixels 
	int pixDepth = ( maxval < 256 ? 1 : 2 );
	int numPixels = width * height * 3;
	if (pixels == NULL)
	{
		pixels = new unsigned char[pixDepth*numPixels];
	}

	size_t n = fread(pixels, sizeof(unsigned char)*pixDepth, numPixels, fp);
	fclose(fp);

	if (n == numPixels)
	{
		//PPM is Most Significant Byte First for 16 bit
		//convert to Little-Endian for PC
		if (pixDepth == 2) SwapBytes(pixels, numPixels, pixDepth);

		return true;
	}
	else
	{
		printf("LoadPGM : fail to read pixels in %s!\n", pathname);
		return false;
	}
}

bool SavePPM(const char *pathname, const unsigned char *pixels, 
			 const int width, const int height, const int maxval)
{
	FILE *fp;
	if ( ( fp = fopen( pathname, "wb" ) ) == NULL )
	{
		printf( "fail to open file %s\n", pathname );
		return false;
	}	

	//write header

	fprintf(fp, "%s\n", "P6");
	fprintf(fp, "%d %d\n", width, height);
	fprintf(fp, "%d\n", maxval);

	//write pixels 
	int pixDepth = ( maxval < 256 ? 1 : 2 );
	int numPixels = width * height * 3;

	if (pixDepth == 2) SwapBytes((unsigned char*)pixels, numPixels, pixDepth);

	size_t n = fwrite(pixels, sizeof(unsigned char)*pixDepth, numPixels, fp);
	fclose(fp);

	if (pixDepth == 2) SwapBytes((unsigned char*)pixels, numPixels, pixDepth);

	return (n == numPixels) ? true : false;
}
