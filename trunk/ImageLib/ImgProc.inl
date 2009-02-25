
template <typename T, typename PROC>
bool pixproc(ImageOf<T> &imq, ImageOf<T> &imp, T &a, PROC &proc)
{
	if (!IsSameSize(&imq,&imp)) return false;

/*
	int imgLen = imq.NumImgPixels();
	int rowLen = imq.NumRowPixels();
	int pixLen = imq.NBands();
*/
	T* p = imp.FirstPixel();
	T* q = imq.FirstPixel();
    const T *pR = p + imp.AllRowStrides();

	const int AC = imp.AllColStrides();
	const int AB = imp.AllBandStrides();

	const int pbs = imp.BandStride();
	const int qbs = imq.BandStride();
	const int pcg = imp.ColGap();
	const int qcg = imq.ColGap();
	const int prg = imp.RowGap();
	const int qrg = imq.RowGap();

	while (p != pR)
	{
		const T *pC = p + AC;
		while (p != pC)
		{
			const T *pB = p + AB;
			while (p != pB)
			{
				PROC::func(*q, *p, a);
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
		PROC::func(*q, *p, a);
		i++;
		p += imp.BandStride();
		q += imq.BandStride();
		if (i%pixLen==0) 
		{
			p += imp.ColGap();
			q += imq.ColGap();
			if (i%rowLen==0)
			{
				p += imp.RowGap();
				q += imq.RowGap();
			}
		}
	}
	*/

	return true;
}

template <typename T>
struct pxAbs { public: static inline void func(T& y, T& x, T& a) { y = (x>0)?x:-x; } };

template <typename T>
struct pxNormInf { public: static inline void func(T& y, T& x, T& a) { a = max(a, abs(y)); } };

template <typename T>
struct pxNorm1 { public: static inline void func(T& y, T& x, T& a) { a += abs(y); } };

template <typename T>
struct pxNorm2 { public: static inline void func(T& y, T& x, T& a) { a += y*y; } };

template <typename T>
struct pxInnerProd { public: static inline void func(T& y, T& x, T& a) { a += y*x; } };

template <typename T>
struct pxPlusAssignS { public: static inline void func(T& y, T& x, T& a) { y += a; } };

template <typename T>
struct pxMinusAssignS { public: static inline void func(T& y, T& x, T& a) { y -= a; } };

template <typename T>
struct pxTimesAssignS { public: static inline void func(T& y, T& x, T& a) { y *= a; } };

template <typename T>
struct pxOverAssignS { public: static inline void func(T& y, T& x, T& a) { y /= a; } };

template <typename T>
struct pxPlusAssign { public: static inline void func(T& y, T& x, T& a) { y += x; } };

template <typename T>
struct pxMinusAssign { public: static inline void func(T& y, T& x, T& a) { y -= x; } };

template <typename T>
struct pxTimesAssign { public: static inline void func(T& y, T& x, T& a) { y *= x; } };

template <typename T>
struct pxOverAssign { public: static inline void func(T& y, T& x, T& a) { y /= x; } };

template <typename T>
struct pxMoveAssign { public: static inline void func(T& y, T& x, T& a) { y += a*x; } };

template <typename T>
void imAbs (ImageOf<T> &dst, ImageOf<T> &src)
{
	dst.ReAllocate(src.Height(), src.Width(), src.NBands());

	T a = T(0);
	pixproc(dst, src, a, pxAbs<T>());
}

template <typename T> 
T imNormInf(ImageOf<T> &img)
{
	T a = T(0);
	pixproc(img, img, a, pxNormInf<T>());
	return a;
}

template <typename T> 
T imNorm1(ImageOf<T> &img)
{
	T a = T(0);
	pixproc(img, img, a, pxNorm1<T>());
	return a/(T)img.NumImgPixels();
}

template <typename T> 
T imNorm2(ImageOf<T> &img)
{
	T a = T(0);
	pixproc(img, img, a, pxNorm2<T>());
	return (T)sqrt(a/(T)img.NumImgPixels());
}

template <typename T>
bool imInnerProd(T &a, ImageOf<T> &im0, ImageOf<T> &im1)
{
	if (!IsSameSize(&im0, &im1)) return false; 

	a = T(0);
	pixproc(im0, im1, a, pxInnerProd<T>());
	return true;
}

template <typename T>
ImageOf<T> & operator += (ImageOf<T> &img, T &a)
{
	pixproc(img, img, a, pxPlusAssignS<T>());
	return img;
}

template <typename T>
ImageOf<T> & operator -= (ImageOf<T> &img, T &a)
{
	pixproc(img, img, a, pxMinusAssignS<T>());
	return img;
}

template <typename T>
ImageOf<T> & operator *= (ImageOf<T> &img, T &g)
{
	T a = T(g);
	pixproc(img, img, a, pxTimesAssignS<T>());
	return img;
}

template <typename T>
ImageOf<T> & operator /= (ImageOf<T> &img, T &g)
{
	T a = T(g);
	pixproc(img, img, a, pxOverAssignS<T>());
	return img;
}

template <typename T>
ImageOf<T> & operator += (ImageOf<T> &imq, ImageOf<T> &imp)
{
	T a = T(0);
	if (IsSameSize(&imq, &imp)) pixproc(imq, imp, a, pxPlusAssign<T>());
	return imq;
}

template <typename T>
ImageOf<T> & operator -= (ImageOf<T> &imq, ImageOf<T> &imp)
{
	T a = T(0);
	if (IsSameSize(&imq, &imp)) pixproc(imq, imp, a, pxMinusAssign<T>());
	return imq;
}

template <typename T>
ImageOf<T> & operator *= (ImageOf<T> &imq, ImageOf<T> &imp)
{
	T a = T(0);
	if (IsSameSize(&imq, &imp)) pixproc(imq, imp, a, pxTimesAssign<T>());
	return imq;
}

template <typename T>
ImageOf<T> & operator /= (ImageOf<T> &imq, ImageOf<T> &imp)
{
	T a = T(0);
	if (IsSameSize(&imq, &imp)) pixproc(imq, imp, a, pxOverAssign<T>());
	return imq;
}

template <typename T>
bool imMoveAssign(ImageOf<T> &dst, ImageOf<T> &src, T &a)
{
	if (!IsSameSize(&dst, &src)) return false;

	pixproc(dst, src, a, pxMoveAssign<T>());
	return true;
}

template <typename T>
bool imFilter(ImageOf<T> &dst, ImageOf<T> &src, const double *knl, int kry, int krx)
//knl is assumed to be in row major
{
	dst.ReAllocate(src.Height(), src.Width(), src.NBands());

	dst.Fill(T(0));

	int ksy = 2*kry+1;
	int ksx = 2*krx+1;
	int H = src.Height()-ksy+1;
	int W = src.Width()-ksx+1;

	ImageOf<T> dst_cnt = dst.SubImage(kry, krx, H, W);	

	for (int y = 0; y < ksy; y++)
	{
		for (int x = 0; x < ksx; x++)
		{
			//double a = 1.0/81.0;
			double a = knl[y*ksx+x];
			//printf("a=%lf\n",a);

			ImageOf<T> src_cnt = src.SubImage(y, x, H, W);

			if (!imMoveAssign(dst_cnt, src_cnt, a)) return false;
		}
	}
	return true;
}

/*
template <typename T>
void imfilter(ImageOf<T> &dst, const ImageOf<T> &src, const double *knl, int kry, int krx)
//knl is assumed to be in row major
{
	if (!IsSameSize(&dst,&src))	dst.ResetPixels(
		new T[src.NumImgPixels()], src.Height(), src.Width(), src.NBands());

	dst.Fill(T(0));

	int ksy = 2*kry+1;
	int ksx = 2*krx+1;
	int H = src.Height()-ksy+1;
	int W = src.Width()-ksx+1;

	ImageOf<T> dst_cnt = dst.SubImage(kry, krx, H, W);	
	int imgLen = dst_cnt.NumImgPixels();
	int rowLen = dst_cnt.NumRowPixels();
	int pixLen = dst_cnt.NBands();

	for (int y = 0; y < ksy; y++)
	{
		for (int x = 0; x < ksx; x++)
		{
			double a = knl[y*ksx+x];

			ImageOf<T> src_cnt = src.SubImage(y, x, H, W);

			const T* p = src_cnt.FirstPixel();
			T* q = dst_cnt.FirstPixel();

			for (int i = 0; i < imgLen;)
			{
				*q += (*p)*a;
				i++;
				p += src_cnt.BandStride();
				q += dst_cnt.BandStride();
				if (i%pixLen==0) 
				{
					p += src_cnt.ColGap();
					q += dst_cnt.ColGap();
					if (i%rowLen==0)
					{
						p += src_cnt.RowGap();
						q += dst_cnt.RowGap();
					}
				}
			}
		}
	}
}
*/

