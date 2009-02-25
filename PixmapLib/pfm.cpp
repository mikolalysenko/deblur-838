#include <string.h>

#include "pixmap.h"

bool LoadPFM(const char *pathname, float *&pixels, 
			 int &width, int &height, float &maxval, int &nbands)
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
	if (strcmp(magic, "PF")==0)	nbands = 3;
	else if (strcmp(magic, "Pf")==0) nbands = 1;
	else 
	{
		printf("LoadPFM : invalid magic number in %s!\n", pathname);
		return false;
	}
	fgetc(fp);

	read_comment(fp);
	fscanf(fp, "%d", &width);
	if (width <= 0)
	{
		printf("LoadPFM : invalid width in %s!\n", pathname);
		return false;
	}
	fgetc(fp);

	read_comment(fp);
	fscanf(fp, "%d", &height);
	if (height <= 0)
	{
		printf("LoadPFM : invalid height in %s!\n", pathname);
		return false;
	}
	fgetc(fp);

	read_comment(fp);
	fscanf(fp, "%f", &maxval);
	if (maxval == 0) 
	{
		printf("LoadPFM : invalid maxval in %s\n", pathname);
		return false;
	}
	fgetc(fp);

	//read pixels 
	int numPixels = width * height * nbands;
	if (pixels == NULL)
	{
		pixels = new float[numPixels];
	}

	size_t n = fread(pixels, sizeof(float), numPixels, fp);
	fclose(fp);

	if (n == numPixels)
	{
		if (maxval>0) SwapBytes((unsigned char*)pixels, numPixels, sizeof(float)); //big endian 
		else maxval = -maxval; //little endian

		return true;
	}
	else
	{
		printf("LoadPGM : fail to read pixels in %s!\n", pathname);
		return false;
	}
}

bool SavePFM(const char *pathname, const float *pixels, 
			 const int width, const int height, const float maxval, const int nbands)
{
	FILE *fp;
	if ( ( fp = fopen( pathname, "wb" ) ) == NULL )
	{
		printf( "fail to open file %s\n", pathname );
		return false;
	}	

	//write header

	if (nbands == 1) fprintf(fp, "%s\n", "Pf");
	else if (nbands == 3) fprintf(fp, "%s\n", "PF");
	else
	{
		printf("unsupported number of bands for PFM!\n");
		return false;
	}
	fprintf(fp, "%d %d\n", width, height);
	fprintf(fp, "%f\n", -maxval);

	//write pixels 
	int numPixels = width * height * nbands;

	size_t n = fwrite(pixels, sizeof(float), numPixels, fp);
	fclose(fp);

	return (n == numPixels) ? true : false;
}
