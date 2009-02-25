#ifndef MVGCUT_H
#define MVGCUT_H

#define INVALID_LABEL -1
#define INFINITY 10000000
#define CUTOFF 1000

enum {RL, BRL, JBRL};

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

	/********** method *********************************/
	int    pMethod;

	void Parse(
		int argc, 
		const char *argv[]);
};

void ReadKernel(
	ImageOf<double> &kernelImg,
	const Parameters &params);

void Conv(
	const ImageOf<double> &image,
	const ImageOf<double> &kernel,
	ImageOf<double>       &result);

/**
 * Richardons-Lucy deconvolution method.
 */
void DeconvRL(
	const ImageOf<double> &inImage,
	ImageOf<short> &outImage,
	const ImageOf<double> &kernelImg,
	const Parameters &params);

#include "deconv.inl"

#endif 




