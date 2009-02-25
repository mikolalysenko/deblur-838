#ifndef CAMERA_H
#define CAMERA_H

#include "../MatrixLib/TVecMat.h"
#include "Image.h"

struct Camera
{
	SqMat3double _rotation;
	Vec3double   _translation;
	SqMat3double _intrinsicMat;

	Vec2double _focalLength;
	Vec2double _principalPnt;
	double _skew;

	Vec<5, double> _distortion;

	void NormalizedCoord2PixelCoord(double &xp, double &yp, double xn, double yn) const 
	{
		xp = _focalLength[0] * ( xn + _skew * yn ) + _principalPnt[0];
		yp = _focalLength[1] * yn + _principalPnt[1];
	}

	void PixelCoord2NormalizedCoord(double &xn, double &yn, double xp, double yp) const
	{
		yn = ( yp - _principalPnt[1] ) / _focalLength[1];
		xn = ( xp - _principalPnt[0] ) / _focalLength[0] - _skew * yn;
	}

	void ApplyDistortion(double &xd, double &yd, double xn, double yn) const 
	{
		double xnyn = xn * yn;
		double xn2	= xn * xn;
		double yn2	= yn * yn;
		double r2	= xn2 + yn2;
		double s = 1.0 + r2 * ( _distortion[0] + r2 * ( _distortion[1] + r2 * _distortion[4] ) );
		double dx = 2 * _distortion[2] * xnyn + _distortion[3] * ( r2 + 2 * xn2 );
		double dy = _distortion[2] * ( r2 + 2 * yn2 ) + 2 * _distortion[3] * xnyn; 
		xd = s * xn + dx;
		yd = s * yn + dy;
	}

	Vec2double DistortedPixelCoord(const Vec2double &xp) const
	{
		double xn, yn;
		PixelCoord2NormalizedCoord(xn, yn, xp[0], xp[1]);
		double xd, yd;
		ApplyDistortion(xd, yd, xn, yn);
		Vec2double res;
		NormalizedCoord2PixelCoord(res[0], res[1], xd, yd);
		return res;
	}

	void xy2XY(double &X, double &Y, double x, double y, double Z) const
	{
		PixelCoord2NormalizedCoord(X, Y, x, y);
		X *= Z;
		Y *= Z;
	}

	void Parameter2Matrix(void);
	void Matrix2Parameter(void);

	void LoadFromFile(const char name[]);
	void SaveToFile(const char name[]);
};

istream &operator >> (istream &ifs, Camera &cam);
ostream &operator << (ostream &ofs, const Camera &cam);

#endif
