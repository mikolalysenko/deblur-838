#ifndef Image_h
#define Image_h

#include <string>
using std::string;

#include <algorithm>
using std::swap;

#include <typeinfo>

#include "../RefCntMem/RefCntMem.h"

class Image //row-major image using reference count memory
{
protected:

	int _nRows, _nCols, _nBands;		
	// the stride and gap are in the unit of pixel type, not byte!
	int _bandStride, _colStride, _rowStride;

	int _allBandStrides, _allColStrides, _allRowStrides;
	int _colGap, _rowGap;	
	int _nRowPixels, _nImgPixels;

	string _name;

protected:

	void ComputeGaps()
	{
		_allBandStrides   = _nBands*_bandStride;
		_allColStrides    = _nCols*_colStride;
		_allRowStrides    = _nRows*_rowStride;

		_colGap		= _colStride-_allBandStrides;
		_rowGap		= _rowStride-_allColStrides;
		
		_nRowPixels = _nCols*_nBands;
		_nImgPixels = _nRows*_nRowPixels;
	}

	void FreePixels()
	{
		_name.clear();
		_nRows = _nCols = _nBands = 0;
		_rowStride = _colStride = _bandStride = 0;
		ComputeGaps();
	}

public:

	Image() { FreePixels(); }
//	Image(const Image &img) { *this = img; }

	bool Empty() const { return _nImgPixels == 0; }

	int Height() const { return _nRows; }
	int Width() const { return _nCols; }
	int NBands() const { return _nBands; }

	int BandStride() const { return _bandStride; }
	int ColStride() const { return _colStride; }
	int RowStride() const { return _rowStride; }

	int AllBandStrides() const { return _allBandStrides; }
	int AllColStrides() const { return _allColStrides; }
	int AllRowStrides() const { return _allRowStrides; }
	int ColGap() const { return _colGap; }
	int RowGap() const { return _rowGap; }	
	int NumRowPixels() const { return _nRowPixels; }
	int NumImgPixels() const { return _nImgPixels; }

	const string& Name() const { return _name; }
	string& Name() { return _name; }
};

inline bool IsSameHeightAndWidth(const Image *I, const Image *J)
{ return I->Height() == J->Height() && I->Width() == J->Width(); }

inline bool IsSameSize(const Image *I, const Image *J)
{ return IsSameHeightAndWidth(I, J) && I->NBands() == J->NBands(); }

template <typename T>
class ImageOf : public Image
{
protected:

	RefCntMem<T> _pixels;    // reference counted memory
	T *_firstPixel;

protected:

	void MakeSubImage(int yO, int xO, int h, int w)
	{ 		
		_firstPixel = _firstPixel + yO * RowStride() + xO * ColStride(); 
		_nRows = h; _nCols = w; ComputeGaps();
	}

	void MakeBandImage(int b)
	{ 		
		_firstPixel = _firstPixel + b * BandStride(); 
		_nBands = 1; ComputeGaps(); 
	}

	void SubSampleImage(int ry, int rx)
	{
		_rowStride *= ry; _nRows = (_nRows-1)/ry+1; 
		_colStride *= rx; _nCols = (_nCols-1)/rx+1; 
		ComputeGaps();
	}

	void MakeTranspose(void)
	{ 
		swap(_colStride, _rowStride); 
		swap(_nCols, _nRows); ComputeGaps(); 
	}

public:
	ImageOf():Image() 
	{
		_firstPixel = 0;
		_pixels.ResetCount(0);
	}
//	ImageOf(const ImageOf<T> &img):Image() { *this = img; }

	void FreePixels()
	{
		_firstPixel = 0;
		_pixels.ResetCount(0);
		Image::FreePixels();
	}

	void ResetPixels(T *pixels, 
		int height, int width, int nbands,
		int bandStride, int colStride, int rowStride)
	{
		_pixels.ResetCount(pixels);
		_firstPixel = pixels;
		
		_nBands = nbands;
		_nCols	= width;
		_nRows  = height;

		_bandStride = bandStride;
		_colStride  = colStride;
		_rowStride  = rowStride;

		ComputeGaps();
	}
	void ResetPixels(T *pixels, int height, int width, int nbands)
	{ 
		ResetPixels(pixels, height, width, nbands, 1, nbands, nbands*width); 
	}
	void ReAllocate(int height, int width, int nbands)
	{ 
		if (height!=Height()||width!=Width()||nbands!=NBands()) 
			ResetPixels(new T[height*width*nbands], height, width, nbands); 
	}

	bool IsMemContinuous() const 
	{ 
		return (_firstPixel && BandStride() == 1 && ColGap() == 0 && RowGap() == 0); 
	}

	T* FirstPixel() { return (T*)_firstPixel; }
	const T* FirstPixel() const { return (const T*)_firstPixel; }
	
	T* PixelAddress(int y, int x, int b) 
	{ return FirstPixel()+y*RowStride()+x*ColStride()+b*BandStride(); }
	const T* PixelAddress(int y, int x, int b) const 
	{ return FirstPixel()+y*RowStride()+x*ColStride()+b*BandStride(); }

	T& Pixel(int y, int x, int b) 
	{ return *(FirstPixel()+y*RowStride()+x*ColStride()+b*BandStride()); }
	const T& Pixel(int y, int x, int b) const 
	{ return *(FirstPixel()+y*RowStride()+x*ColStride()+b*BandStride()); }

	T* PixelAddress(int y, int x) 
	{ return FirstPixel()+y*RowStride()+x*ColStride(); }
	const T* PixelAddress(int y, int x) const 
	{ return FirstPixel()+y*RowStride()+x*ColStride(); }

	T& Pixel(int y, int x) 
	{ return *(FirstPixel()+y*RowStride()+x*ColStride()); }
	const T& Pixel(int y, int x) const 
	{ return *(FirstPixel()+y*RowStride()+x*ColStride()); }

	ImageOf<T> SubImage(int y, int x, int height, int width) const
	{ ImageOf<T> retval = *this; retval.MakeSubImage(y, x, height, width); return retval; }

	ImageOf<T> SubSampledImage(int ry, int rx) const
	{ ImageOf<T> retval = *this; retval.SubSampleImage(ry, rx); return retval; }

	ImageOf<T> BandImage(int band) const
	{ ImageOf<T> retval = *this; retval.MakeBandImage(band); return retval; }

	ImageOf<T> Transpose() const
	{ ImageOf<T> retval = *this; retval.MakeTranspose(); return retval; }

	inline void Fill(const T& a);
	inline void Clamp(const T& m, const T&M);
};

template <typename D, typename S>
void CopyPixels(ImageOf<D>& dst, const ImageOf<S>& src);

template <typename T>
bool LoadPixels(const char *name, ImageOf<T> &img);

template <typename T>
bool SavePixels(const char *name, ImageOf<T> &img);

template <typename T>
bool WritePixels(const char *name, const ImageOf<T> &img);

template <typename T>
bool ReadPixels(const char *name, ImageOf<T> &img);

#include "Image.inl"
#include "ImgIO.inl"

#include "ImgProc.h"
#include "ImgMath.h"

#endif 



