#ifndef ImgIO_h
#define ImgIO_h

#include "../PixmapLib/pixmap.h"
#include "../BitmapLib/bitmap.h"

template <typename D, typename S>
void CopyPixels(ImageOf<D>& dst, const ImageOf<S>& src)
{
	dst.ReAllocate(src.Height(), src.Width(), src.NBands());

	//double mv,MV;
	//Image::PixelRange(&typeid(D), mv, MV);

/*
	int imgLen = dst.NumImgPixels();
	int rowLen = dst.NumRowPixels();
	int pixLen = dst.NBands();
*/
	const S* p = src.FirstPixel();
	D* q = dst.FirstPixel();
	const S *pR = p + src.AllRowStrides();

	const int AC = src.AllColStrides();
	const int AB = src.AllBandStrides();

	const int pbs = src.BandStride();
	const int qbs = dst.BandStride();
	const int pcg = src.ColGap();
	const int qcg = dst.ColGap();
	const int prg = src.RowGap();
	const int qrg = dst.RowGap();
	
	while (p != pR)
	{
		const S *pC = p + AC;
		while (p != pC)
		{
			const S *pB = p + AB;
			while (p != pB)
			{
				//*q = (D)min(MV, max(mv, (double)(*p)));
				*q = (D)*p;
				p += pbs;
				q += qbs;
			}
			p += pcg;
			q += qcg;
		}
		p += prg;
		q += qrg;
	}
/*
	for (int i = 0; i < imgLen;)
	{
		*q = (D)min(MV, max(mv, (double)(*p)));
		i++;
		p += src.BandStride();
		q += dst.BandStride();
		if (i%pixLen==0) 
		{
			p += src.ColGap();
			q += dst.ColGap();
			if (i%rowLen==0)
			{
				p += src.RowGap();
				q += dst.RowGap();
			}
		}
	}
*/
}

template <typename T>
bool WritePixels(const char *name, const ImageOf<T> &img)
{
	FILE *fp = fopen(name, "wb");

	if (fp==NULL) 
	{
		printf("WritePixels: fail to open %s\n", name);
		return false;
	}

	ImageOf<T> tmp;

	if (img.IsMemContinuous()) tmp = img;
	else CopyPixels(tmp, img);
	
	int W = img.Width();
	int H = img.Height();
	int B = img.NBands();

	int n;
	n = (int)fwrite(&H, sizeof(int), 1, fp);
	if (n != 1)
	{
		printf("WritePixels: fail to write height field in %s\n", name);
		return false;
	}

	n = (int)fwrite(&W, sizeof(int), 1, fp);
	if (n != 1)
	{
		printf("WritePixels: fail to write width field in %s\n", name);
		return false;
	}

	n = (int)fwrite(&B, sizeof(int), 1, fp);
	if (n != 1)
	{
		printf("WritePixels: fail to write band field in %s\n", name);
		return false;
	}

	n = (int)fwrite(tmp.FirstPixel(), sizeof(T), tmp.NumImgPixels(), fp);
	if (n != tmp.NumImgPixels())
	{
		printf("WritePixels: fail to write image data field in %s\n", name);
		return false;
	}
	
	fclose(fp);

	return true;
}

template <typename T>
bool ReadPixels(const char *name, ImageOf<T> &img)
{
	FILE *fp = fopen(name, "rb");

	if (fp==NULL) 
	{
		printf("ReadPixels: fail to open %s\n", name);
		return false;
	}

	int W, H, B;
	int n;

	n = (int)fread(&H, sizeof(int), 1, fp);
	if (n != 1)
	{
		printf("ReadPixels: fail to read height field in %s\n", name);
		return false;
	}

	n = (int)fread(&W, sizeof(int), 1, fp);
	if (n != 1)
	{
		printf("ReadPixels: fail to read width field in %s\n", name);
		return false;
	}

	n = (int)fread(&B, sizeof(int), 1, fp);
	if (n != 1)
	{
		printf("ReadPixels: fail to read band field in %s\n", name);
		return false;
	}

	if (!img.IsMemContinuous()) img.ReAllocate(H, W, B);

	n = (int)fread(img.FirstPixel(), sizeof(T), H*W*B, fp);
	if (n != H*W*B)
	{
		printf("ReadPixels: fail to read image data field in %s\n", name);
		return false;
	}

	fclose(fp);

	return true;

}

template <typename T>
bool LoadPixels(const char *name, ImageOf<T> &img)
{
	int L = (int)strlen(name);
	const char *ext = name+L-3;

	if (strcmp(ext,"PGM")==0 || strcmp(ext,"pgm")==0)
	{
		int width, height, nbands;
		unsigned char *pixels = 0;	
		int maxval;
		if (LoadPGM(name, pixels, width, height, maxval))
		{
			nbands = 1;
			if (maxval>255)
			{
				ImageOf<unsigned short> buf;
				buf.ResetPixels((unsigned short*)pixels, height, width, nbands);
				CopyPixels(img, buf);
			}
			else 
			{
				ImageOf<unsigned char> buf;
				buf.ResetPixels(pixels, height, width, nbands);
				CopyPixels(img, buf);
			}
		}
		else
		{
			printf("fail to load: %s\n", name);
			return false;
		}
	}
	else if (strcmp(ext,"PPM")==0 || strcmp(ext,"ppm")==0)
	{
		int width, height, nbands;
		unsigned char *pixels = 0;	
		int maxval;
		if (LoadPPM(name, pixels, width, height, maxval))
		{
			nbands = 3;
			if (maxval>255)
			{
				ImageOf<unsigned short> buf;
				buf.ResetPixels((unsigned short*)pixels, height, width, nbands);
				CopyPixels(img, buf);
			}
			else 
			{
				ImageOf<unsigned char> buf;
				buf.ResetPixels(pixels, height, width, nbands);
				CopyPixels(img, buf);
			}
		}
		else
		{
			printf("fail to load: %s\n", name);
			return false;
		}
	}
	else if (strcmp(ext,"PFM")==0 || strcmp(ext,"pfm")==0)
	{
		int width, height, nbands;
		float *pixels = 0;	
		float maxval;
		if (LoadPFM(name, pixels, width, height, maxval, nbands))
		{
			ImageOf<float> buf;
			buf.ResetPixels(pixels, height, width, nbands);
			CopyPixels(img, buf);
		}
		else
		{
			printf("fail to load: %s\n", name);
			return false;
		}
	}
	else if (strcmp(ext,"BMP")==0 || strcmp(ext,"bmp")==0)
	{
		int width, height, nbands;
		unsigned char *pixels = 0;	
		int alignment = 1;
		bool top2bot = true;
		if (LoadBMP(name, pixels, width, height, nbands, alignment, top2bot))
		{
			ImageOf<unsigned char> buf;
			buf.ResetPixels(pixels, height, width, nbands);
			CopyPixels(img, buf);
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

	img.Name() = name;
	
	return true;
}

template <typename T>
bool SavePixels(const char *name, ImageOf<T> &img)
{
	int L = (int)strlen(name);
	const char *ext = name+L-3;

	int width = img.Width();
	int height = img.Height();
	int nbands = img.NBands();

	if (strcmp(ext,"PGM")==0 || strcmp(ext,"pgm")==0)
	{
		if (nbands != 1) 
		{
			printf("%s must have 1 color band!\n",name);
			return false;
		}
		int maxval;
		if (img.IsMemContinuous() && typeid(T)==typeid(unsigned char))
		{
			maxval = 255;
			unsigned char *pixels = (unsigned char*)img.FirstPixel();	
			SavePGM(name, pixels, width, height, maxval);	
		}
		else if (img.IsMemContinuous() && typeid(T)==typeid(unsigned short))
		{
			maxval = 65535;
			unsigned short *pixels = (unsigned short*)img.FirstPixel();				
			SavePGM(name, (unsigned char*)pixels, width, height, maxval);
		}
		else 
		{
			maxval = 255;
			ImageOf<unsigned char> buf;
			CopyPixels(buf, img);
			SavePGM(name, buf.FirstPixel(), width, height, maxval);	
		}
	}
	else if (strcmp(ext,"PPM")==0 || strcmp(ext,"ppm")==0)
	{
		if (nbands != 3) 
		{
			printf("%s must have 3 color band!\n",name);
			return false;
		}
		int maxval;
		if (img.IsMemContinuous() && typeid(T)==typeid(unsigned char))
		{
			maxval = 255;
			unsigned char *pixels = (unsigned char*)img.FirstPixel();
			SavePPM(name, pixels, width, height, maxval);	
		}
		else if (img.IsMemContinuous() && typeid(T)==typeid(unsigned short))
		{
			maxval = 65535;
			unsigned short *pixels = (unsigned short*)img.FirstPixel();	
			SavePPM(name, (unsigned char*)pixels, width, height, maxval);
		}
		else 
		{
			maxval = 255;
			ImageOf<unsigned char> buf;
			CopyPixels(buf, img);
			SavePPM(name, buf.FirstPixel(), width, height, maxval);	
		}
	}
	else if (strcmp(ext,"PFM")==0 || strcmp(ext,"pfm")==0)
	{
		float maxval = 1.0f;
		if (img.IsMemContinuous() && typeid(T)==typeid(float))
		{
			float *pixels = (float*)img.FirstPixel();
			SavePFM(name, pixels, width, height, maxval, nbands);	
		}
		else 
		{
			ImageOf<float> buf;
			CopyPixels(buf, img);
			SavePFM(name, buf.FirstPixel(), width, height, maxval, nbands);	
		}
	}
	else if (strcmp(ext,"BMP")==0 || strcmp(ext,"bmp")==0)
	{
		if (nbands != 1 && nbands != 3)
		{
			printf("%s must have 1 or 3 bands!\n",name);
			return false;
		}

		int alignment = 1;
		bool top2bot = true;
		if (img.IsMemContinuous() && typeid(T)==typeid(unsigned char)) 
		{
			unsigned char *pixels = (unsigned char *)img.FirstPixel();
			SaveBMP(name, pixels, width, height, nbands, alignment, top2bot);	
		}
		else
		{			
			ImageOf<unsigned char> buf;
			CopyPixels(buf, img);
			SaveBMP(name, buf.FirstPixel(), width, height, nbands, alignment, top2bot);	
		}
	}
	else
	{
		printf("unsupported format: %s\n", name);
		return false;
	}
	return true;
}

#endif

