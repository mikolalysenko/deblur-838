#ifndef Image_inl
#define Image_inl

template <typename T>
void ImageOf<T>::Fill(const T& a)
{
	T *p = FirstPixel();
	const T *pR = p + AllRowStrides();

	const int AC = AllColStrides();
	const int AB = AllBandStrides();

	const int pbs = BandStride();
	const int pcg = ColGap();
	const int prg = RowGap();

	while (p != pR)
	{
		const T *pC = p + AC;
		while (p != pC)
		{
			const T *pB = p + AB;
			while (p != pB)
			{
				*p = a;
				p += pbs;
			}
			p += pcg;
		}
		p += prg;
	}
	/*
	if (p)
	{
		for (int i = 0; i < NumImgPixels();)
		{
			*p = a;
			i++;
			p += BandStride();
			if (i%NBands()==0) { p += ColGap(); if (i%NumRowPixels()==0) p+= RowGap(); }
		}
	}
	*/
}

template <typename T>
void ImageOf<T>::Clamp(const T& m, const T&M)
{
	T *p = FirstPixel();
	const T *pR = p + AllRowStrides();

	const int AC = AllColStrides();
	const int AB = AllBandStrides();

	const int pbs = BandStride();
	const int pcg = ColGap();
	const int prg = RowGap();

	while (p != pR)
	{
		const T *pC = p + AC;
		while (p != pC)
		{
			const T *pB = p + AB;
			while (p != pB)
			{
				*p = min(M, max(m, *p));
				p += pbs;
			}
			p += pcg;
		}
		p += prg;
	}
	/*
	if (p)
	{
		for (int i = 0; i < NumImgPixels();)
		{
			*p = a;
			i++;
			p += BandStride();
			if (i%NBands()==0) { p += ColGap(); if (i%NumRowPixels()==0) p+= RowGap(); }
		}
	}
	*/
}

#endif

