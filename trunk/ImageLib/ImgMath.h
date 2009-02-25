#ifndef ImgMath_h
#define ImgMath_h

inline double ResampleLinear(double v0, double v1, double f)
{
    return v0 + f * (v1 - v0);
}

inline double ResampleCubicV(double v0, double v1, double v2, double v3, double f)
{
	// 0.5 * [f^3 f^2 f^1 f^0] * [-1 3 -3 1; 2 -5 4 -1; -1 0 1 0; 0 2 0 0] * [v0; v1; v2; v3]
	double v0_2 = v0+v0;

	double v1_2 = v1+v1;
	double v1_3 = v1_2+v1;
	double v1_5 = v1_2+v1_3;
	
	double v2_2 = v2+v2;
	double v2_3 = v2_2+v2;
	double v2_4 = v2_3+v2;

    double a0 = (v1_3+v3)-(v0+v2_3);
	double a1 = (v0_2+v2_4)-(v1_5+v3);
	double a2 = v2-v0;
	double a3 = v1_2;

	double v = 0.5*(((a0*f+a1)*f+a2)*f+a3);
	return v;
}

inline double ResampleCubicD(double v0, double v1, double v2, double v3, double f)
{
	// 0.5 * [f^3 f^2 f^1 f^0] * [-1 3 -3 1; 2 -5 4 -1; -1 0 1 0; 0 2 0 0] * [v0; v1; v2; v3]
	double v0_2 = v0+v0;

	double v1_2 = v1+v1;
	double v1_3 = v1_2+v1;
	double v1_5 = v1_2+v1_3;
	
	double v2_2 = v2+v2;
	double v2_3 = v2_2+v2;
	double v2_4 = v2_3+v2;

    double a0 = (v1_3+v3)-(v0+v2_3);
	double a1 = (v0_2+v2_4)-(v1_5+v3);
	double a2 = v2-v0;
	double a3 = v1_2;

	// v = 0.5*(((a0*f+a1)*f+a2)*f+a3);
	// d = 0.5*(((a0*f+a1)*f+a2)+((a0*f+a1)+(a0)*f)*f) 
	//	 = 0.5*(3*a0*f^2 + 2*a1*f + a2)
	return 0.5 * ( ( 3 * a0 * f + 2 * a1 ) * f + a2 );
}

inline void ResampleCubicVD(double &v, double &d, double v0, double v1, double v2, double v3, double f)
{
	// 0.5 * [f^3 f^2 f^1 f^0] * [-1 3 -3 1; 2 -5 4 -1; -1 0 1 0; 0 2 0 0] * [v0; v1; v2; v3]
	double v0_2 = v0+v0;

	double v1_2 = v1+v1;
	double v1_3 = v1_2+v1;
	double v1_5 = v1_2+v1_3;
	
	double v2_2 = v2+v2;
	double v2_3 = v2_2+v2;
	double v2_4 = v2_3+v2;

    double a0 = (v1_3+v3)-(v0+v2_3);
	double a1 = (v0_2+v2_4)-(v1_5+v3);
	double a2 = v2-v0;
	double a3 = v1_2;

	// v = 0.5*(((a0*f+a1)*f+a2)*f+a3);
	// d = 0.5*(((a0*f+a1)*f+a2)+((a0*f+a1)+(a0)*f)*f) 
	//	 = 0.5*(3*a0*f^2 + 2*a1*f + a2)
	double a0f		= a0 * f;
	double a0f1		= a0f + a1;
	double a0f1f	= a0f1 * f;
	double a0f1f2	= a0f1f + a2;
	double a0f1f2f	= a0f1f2 * f;
	double a0f1f2f3 = a0f1f2f + a3;

	v = 0.5 * a0f1f2f3;
	d = 0.5 * (a0f1f2 + (a0f1 + a0f)*f);	
}

template <class T>
inline T ResampleBiLinear(T *src, int oH, int oV, double xf, double yf)
{
    // Resample a pixel using bilinear interpolation
    double h1 = ResampleLinear(src[0 ], src[   oH], xf);
    double h2 = ResampleLinear(src[oV], src[oV+oH], xf);
    double  v = ResampleLinear(h1, h2, yf);
    return (T)v;
} 

template <class T>
inline T ResampleBiCubic(T *src, int oH, int oV, double xf, double yf)
{
    // Resample a pixel using bilinear interpolation
    double h[4];
    for (int i = 0; i < 4; i++)
    {
        int j = (i-1)*oV;
        h[i] = ResampleCubicV(src[j-oH], src[j], src[j+oH], src[j+2*oH], xf);
    }
    double  v = ResampleCubicV(h[0], h[1], h[2], h[3], yf);
    return (T)v;
}

#endif

