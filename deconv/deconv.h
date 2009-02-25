#ifndef MVGCUT_H
#define MVGCUT_H

#include <stdio.h>
#include <float.h>
#include <math.h>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

#include "../CmdIOLib/CmdArgParser.h"
#include "../ImageLib/Image.h"

enum {RL, BRL, JBRL};
#define NOTSET -1

struct Coord
{
	int x, y;

	Coord() {}
	Coord(int a, int b) { x = a; y = b; }

	Coord operator- ()        { return Coord(-x, -y); }
	Coord operator+ (Coord a) { return Coord(x + a.x, y + a.y); }
	Coord operator- (Coord a) { return Coord(x - a.x, y - a.y); }
	bool  operator< (Coord a) { return (x <  a.x) && (y <  a.y); }
	bool  operator<=(Coord a) { return (x <= a.x) && (y <= a.y); }
	bool  operator> (Coord a) { return (x >  a.x) && (y >  a.y); }
	bool  operator>=(Coord a) { return (x >= a.x) && (y >= a.y); }
	bool  operator==(Coord a) { return (x == a.x) && (y == a.y); }
	bool  operator!=(Coord a) { return (x != a.x) || (y != a.y); }
};

struct Parameters
{
	/********** input and output file names ************/
	string pImKernelFile;
	string pInImgFile;
	string pOutImgFile;

	/********** number of iterations *******************/
	int    pNumIters;

	/********** deconvolution method ********************/
	int    pMethod;

	/*
	 * Regularization factor (lambda in the Yuan et al. paper);
	 * defaults to 1, which is probably not the best value.
	 */
	double  pRegFactor;

	/*
	 * Regularization neighborhood radius; if not set, then
	 * defaults to 0.5*(radius of kernel)
	 */
	int  pNbrRadius;  

	/*
	 * Spatial variance; if not set, then defaults to (pNbrRadius/3)^2.
	 */
	double  pSpatialVar;

	/*
	 * Intensity range variance; if not set, then defaults to 
	 * 0.01*abs(max(corruptedImg(:)) - min(corruptedImg(:)))^2.
	 */
	double  pIntRngVar;

	void Parse(
		int argc, 
		const char *argv[]);
};

void ReadKernel(
	ImageOf<float> &kernelImg,
	const Parameters &params);

void Conv(
	const ImageOf<float> &image,
	const ImageOf<float> &kernel,
	ImageOf<float>       &result);

/**
 * Richardons-Lucy deconvolution method.
 */
void DeconvRL(
	const ImageOf<float> &inImage,
	ImageOf<short> &outImage,
	const ImageOf<float> &kernelImg,
	const Parameters &params);

/**
 * Bilateral Richardson-Lucy method for image deconvolution.
 */
void DeconvBRL(
	const ImageOf<float> &inImage,
	ImageOf<short> &outImage,
	const ImageOf<float> &kernelImg,
	const Parameters &params);

#include "deconv.inl"

#endif 




