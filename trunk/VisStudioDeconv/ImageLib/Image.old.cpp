#include "../PixmapLib/pixmap.h"
#include "../BitmapLib/bitmap.h"
#include "Image.h"

bool LoadPixels(const char *name, Image &img)
{
	int L = (int)strlen(name);
	const char *ext = name+L-3;

	int width, height, nbands;
	unsigned char *pixels = (unsigned char*)img.FirstPixel();
	const type_info *type; 

	if (strcmp(ext,"PGM")==0 || strcmp(ext,"pgm")==0)
	{
		int maxval;
		if (LoadPGM(name, pixels, width, height, maxval))
		{
			nbands = 1;
			type = (maxval>255) ? (&typeid(unsigned short)) : (&typeid(unsigned char));			
		}
		else
		{
			printf("fail to load: %s\n", name);
			return false;
		}
	}
	else if (strcmp(ext,"PPM")==0 || strcmp(ext,"ppm")==0)
	{
		int maxval;
		if (LoadPPM(name, pixels, width, height, maxval))
		{
			nbands = 3;
			type = (maxval>255) ? (&typeid(unsigned short)) : (&typeid(unsigned char));			
		}
		else
		{
			printf("fail to load: %s\n", name);
			return false;
		}
	}
	else if (strcmp(ext,"BMP")==0 || strcmp(ext,"bmp")==0)
	{
		int alignment = 1;
		bool top2bot = true;
		if (LoadBMP(name, pixels, width, height, nbands, alignment, top2bot))
		{
			type = &typeid(unsigned char);
		}
		else
		{
			printf("fail to load: %s\n", name);
			return false;
		}
	}
	else
	{
		printf("unsupported format: %s\n", name);
		return false;
	}

	if (!img.FirstPixel())
	{
		img.ResetPixels(pixels, type, height, width, nbands, 1, nbands, nbands*width);
	}
	img.Name() = name;
	
	return true;
}

bool SavePixels(const char *name, const Image &img)
{
	if (!img.IsMemContinuous())
	{
		printf("%s must be continuous in memory!\n", name);
		return false;
	}

	int L = (int)strlen(name);
	const char *ext = name+L-3;

	int width = img.Width();
	int height = img.Height();
	int nbands = img.NBands();
	unsigned char *pixels = (unsigned char*)img.FirstPixel();
	const type_info *type = img.TypeInfo();

	if (strcmp(ext,"PGM")==0 || strcmp(ext,"pgm")==0)
	{
		if (nbands != 1) 
		{
			printf("%s must have 1 color band!\n",name);
			return false;
		}
		int maxval = (*type==typeid(unsigned char)) ? 255 : 65535;
		SavePGM(name, pixels, width, height, maxval);		
	}
	else if (strcmp(ext,"PPM")==0 || strcmp(ext,"ppm")==0)
	{
		if (nbands != 3) 
		{
			printf("%s must have 3 color band!\n",name);
			return false;
		}
		int maxval = (*type==typeid(unsigned char)) ? 255 : 65535;
		SavePPM(name, pixels, width, height, maxval);		
	}
	else if (strcmp(ext,"BMP")==0 || strcmp(ext,"bmp")==0)
	{
		if (*type != typeid(unsigned char))
		{
			printf("%s must have 1 byte per band!\n",name);
			return false;
		}
		int alignment = 1;
		bool top2bot = true;
		SaveBMP(name, pixels, width, height, nbands, alignment, top2bot);		
	}
	else
	{
		printf("unsupported format: %s\n", name);
		return false;
	}
	return true;
}
