#include <stdio.h>
#include <float.h>
#include <math.h>
//#include <valarray>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;
//using std::valarray;

#include "../CmdIOLib/CmdArgParser.h"
//#include "../MatrixLib/PVecMat.h"
//#include "../MatrixLib/TVecMat.h"
#include "../ImageLib/Image.h"

#include "deconv.h"



void Parameters::Parse(
	int argc, 
	const char *argv[])
{
	vector<string> allArgv;

	collectAllArgs(allArgv, argc, argv);

	if (!extractUnknownParaFromCommandArg(pImKernelFile, "kernel image file", "-kernelFile", allArgv)) exit(0);

	if (!extractUnknownParaFromCommandArg(pInImgFile,    "input (blurry) image file", "-inputFile", allArgv)) exit(0);

	pOutImgFile = "deconf_rl_result.ppm";
	extractDefaultParaFromCommandArg(pOutImgFile,        "output (deblurred) image file", "-outputFile", allArgv);

	pNumIters = 50;
	extractDefaultParaFromCommandArg(pNumIters,          "number of iterations", "-numIters", allArgv);
	if(pNumIters <= 0)
	{
		printf("Error: Number of iterations must be greater than 0!\n");
		exit(0);
	}

	string method = "RL";
	extractDefaultParaFromCommandArg(method,          "deconvolution method", "-method", allArgv);
	if(method.compare("RL") == 0 || method.compare("rl") == 0)
	{
		pMethod = RL;
	}
	else if(method.compare("BRL") == 0 || method.compare("brl") == 0)
	{
		pMethod = BRL;
	}
	else if(method.compare("JBRL") == 0 || method.compare("jbrl") == 0)
	{
		pMethod = JBRL;
	}
	else
	{
		printf("Error: %s is not a valid deconvolution method (choose RL, BRL, or JBRL)!\n", method.c_str());
		exit(0);
	}
}

void ReadKernel(
	ImageOf<double> &kernelImg,
	const Parameters &params)
{
	printf("Reading %s ... ", params.pImKernelFile.c_str());

	if(params.pImKernelFile.find(".txt") != string::npos)
	{
		// kernel file is txt

		double val;

		ifstream inFile;
		inFile.open(params.pImKernelFile.c_str());
		if(!inFile)
		{
			printf("Error: Unable to open file %s\n", params.pImKernelFile.c_str());
			exit(0);
		}

		int lineCount = 0;
		int pCount = 0;
		int height = 0;
		int width = 0;
		while(inFile >> val)
		{
			if(lineCount == 0)
			{
				height = (int)val;
			}
			else if(lineCount == 1)
			{
				width  = (int)val;
				kernelImg.ReAllocate(height, width, 1);
			}
			else
			{
				int y = pCount % height;
				int x = (int)floor((double)pCount / (double)height);

				kernelImg.Pixel(y,x) = val;
				
				pCount++;

				if(pCount >= height*width)
				{
					break;
				}
			}
			lineCount++;
		}

		inFile.close();
		printf("done:\n");

		for(int y = 0; y < height; y++)
		{
			printf("\t");
			for(int x = 0; x < width; x++)
			{
				printf("%f ", kernelImg.Pixel(y,x));
			}
			printf("\n");
		}
	}
	else
	{
		// kernel file is other
		printf("Error: cannot currently handle kernel images, only text files!\n");
		exit(0);
	}
}

void Conv(
	const ImageOf<double> &image,
	const ImageOf<double> &kernel,
	ImageOf<double>       &result)
{
	int iheight = image.Height();
	int iwidth = image.Width();
	int inbands = image.NBands();
	int kheight = kernel.Height();
	int kwidth = kernel.Width();
	double knpx   = (double)kheight*kwidth;

	result.ReAllocate(iheight, iwidth, inbands);

	const int center_kx = (int)floor((float)kwidth/ 2.0f);
	const int center_ky = (int)floor((float)kheight/ 2.0f);

	for(int iy = 0; iy < iheight; iy++)
	{
		for(int ix = 0; ix < iwidth; ix++)
		{
			for(int b = 0; b < inbands; b++)
			{
				double cnt = 0;
				double accum = 0;
				int strty = -min(center_ky, iy);
				int stopy =  min(kheight-center_ky, iheight-iy);
				for(int ky = strty; ky < stopy; ky++)
				{
					int strtx = -min(center_kx, ix);
					int stopx =  min(kwidth-center_kx, iwidth-ix);
					for(int kx = strtx; kx < stopx; kx++)
					{
						cnt++;
						accum += image.Pixel(iy+ky, ix+kx, b) * kernel.Pixel(center_ky+ky, center_kx+kx);
					}
				}

				result.Pixel(iy, ix, b) = accum * knpx/cnt;
			}
		}
	}
}


/**
 * Richardson-Lucy method for image deconvolution.
 */
void DeconvRL(
	const ImageOf<double> &inImage,
	ImageOf<short> &outImage,
	const ImageOf<double> &kernelImg,
	const Parameters &params)
{
	int height = inImage.Height();
	int width  = inImage.Width();
	int nbands = inImage.NBands();
	int kheight = kernelImg.Height();
	int kwidth  = kernelImg.Width();

	printf("\nBeginning Richardson-Lucy Deconvolution...\n\n");

	// create adjoint of kernel
	ImageOf<double> adjKernel;
	adjKernel.ReAllocate(kheight, kwidth, 1);
	for(int y = 0; y < kheight; y++)
	{
		for(int x = 0; x < kwidth; x++)
		{
			adjKernel.Pixel(y,x) = kernelImg.Pixel(kheight-y-1, kwidth-x-1);
		}
	}
	// done create adjoint of kernel

	ImageOf<double> reg;
	ImageOf<double> frac;
	ImageOf<double> tmpImage;
	CopyPixels(tmpImage, inImage);
	
	for(int it = 0; it < params.pNumIters; it++)
	{
		int percent_complete = (int)floor((float)it/(float)(params.pNumIters)*100.0f + 0.5f);
		display_progress(percent_complete);

		Conv(tmpImage, adjKernel, reg);
		CopyPixels(frac, inImage);
		frac /= reg;
		Conv(frac, adjKernel, reg);
		tmpImage *= reg;
	}
	display_progress(100);

	outImage.ReAllocate(height, width, nbands);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			for(int b = 0; b < nbands; b++)
			{
				outImage.Pixel(y,x,b) = (short)tmpImage.Pixel(y,x,b);
			}
		}
	}

	printf("\n\n");
}

int main(int argc, const char *argv[])
{
	Parameters params;	// stores all of the input parameters
	params.Parse(argc, argv);

	ImageOf<double> inImage;
	ImageOf<short>  outImage;
	ImageOf<double> kernel;
	ImageOf<double> adjKernel;

	const char *name = params.pInImgFile.c_str();
	printf("Load %s ... ", name);
	if (!LoadPixels(name, inImage)) exit(0);
	printf("done\n");

	ReadKernel(kernel, params);

	switch(params.pMethod)
	{
		case RL:
			DeconvRL(inImage, outImage, kernel, params);
			break;
		case BRL:
			printf("Sorry, BRL is not implemented yet!\n");
			break;
		case JBRL:
			printf("Sorry, JBRL is not implemented yet!\n");
			break;
		default:
			printf("Sorry, method %d is not implemented yet!\n", params.pMethod);
			break;

	}

	
	name = params.pOutImgFile.c_str();
	printf("Save %s ... ", name);
	outImage.Clamp(0,255);
	SavePixels(name, outImage);
	printf("done\n");

	return 0;
}

