#ifndef ImgProc_h
#define ImgProc_h

template <typename T>
inline bool pixproc(ImageOf<T> &imq, ImageOf<T> &imp, T &a, void (*f)(T&, T&, T&a));


template <typename T> 
T imNormInf(ImageOf<T> &img);

template <typename T>
T imNorm1(ImageOf<T> &img);

template <typename T>
T imNorm2(ImageOf<T> &img);

template <typename T>
bool imInnerProd(T &a, ImageOf<T> &im0, ImageOf<T> &im1);


template <typename T>
ImageOf<T> & operator += (ImageOf<T> &imq, ImageOf<T> &imp);

template <typename T>
ImageOf<T> & operator -= (ImageOf<T> &imq, ImageOf<T> &imp);

template <typename T>
ImageOf<T> & operator *= (ImageOf<T> &imq, ImageOf<T> &imp);

template <typename T>
ImageOf<T> & operator /= (ImageOf<T> &imq, ImageOf<T> &imp);


template <typename T>
ImageOf<T> & operator += (ImageOf<T> &img, T &a);

template <typename T>
ImageOf<T> & operator -= (ImageOf<T> &img, T &a);

template <typename T>
ImageOf<T> & operator *= (ImageOf<T> &img, T &a);

template <typename T>
ImageOf<T> & operator /= (ImageOf<T> &img, T &a);


template <typename T>
bool imMoveAssign(ImageOf<T> &dst, ImageOf<T> &src, T &a);


template <typename T>
bool imFilter(ImageOf<T> &dst, ImageOf<T> &src, const double *knl, int kry, int krx);

#include "ImgProc.inl"

#endif

