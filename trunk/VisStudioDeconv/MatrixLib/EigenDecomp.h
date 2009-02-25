#ifndef EigenDecomp_h
#define EigenDecomp_h

#include "TVecMat.h"

inline bool SolveRealQuadricRoots(double z[2], double a1, double a0)
//solve z[0], z[1] for 
//z^2 + a1 * z + a0 = 0
//assuming the equation has 2 real roots
{
	double half_a1	= 0.5 * a1;
	double D		= half_a1 * half_a1 - a0;
	if (D < 0)
	{
		return false;
	}
	else
	{
		double sr_D = sqrt(D);
		z[0] = -half_a1 + sr_D;
		z[1] = -half_a1 - sr_D;
		return true;
	}
}

inline bool SolveRealCubicRoots(double z[3], double a2, double a1, double a0)
//solve z[0], z[1], z[2] for 
//z^3 + a2 * z^2 + a1 * z + a0 = 0
//assuming the equation has 3 real roots
//the solution comes from http://mathworld.wolfram.com/CubicEquation.html
{
	const double PI = 3.1415926535897932384626433832795;
	const double Q	= ( 3.0 * a1 - a2 * a2 ) / 9.0;
	const double R	= ( 9.0 * a2 * a1 - 27.0 * a0 - 2.0 * a2 * a2 * a2 ) / 54.0;
		
	const double Q3 = Q * Q * Q;
	const double R2 = R * R;
	
	const double D	= Q3 + R2;

	if ( D > 0 ) 
	// the equation has complex roots
	{
		return false; 
	}
	else
	{
		double third_a2 = a2 / 3.0;
		if ( Q == 0.0 )
		{
			z[0] = z[1] = z[2] = -third_a2;
		}
		else 
		{
			double sr_mQ	= sqrt( -Q );
			double sr_2mQ	= 2.0 * sr_mQ;			
			double sr_mQ3   = sr_mQ * sr_mQ * sr_mQ;

			if (fabs(sr_mQ3) < FLT_EPSILON) return false;

			double ctheta   = R / sr_mQ3;

			if (ctheta < -1.0 || ctheta > 1.0) return false;

			double theta	= acos(ctheta);

			z[0] = sr_2mQ * cos( theta / 3.0 ) - third_a2;
			z[1] = sr_2mQ * cos( ( theta + 2 * PI ) / 3.0 ) - third_a2;
			z[2] = sr_2mQ * cos( ( theta + 4 * PI ) / 3.0 ) - third_a2;
		}
        
		return true;
	}
}

inline bool SymEigenDecomp(const Vec<3,double> &m, 
						   double eigVal[2], 
						   Vec2double eigVec[2])
//eigen decompose symmetric matrix
//[a, b; 
// b, c]
{
	double a = m[0];
	double b = m[1];
	double c = m[2];

	if (SolveRealQuadricRoots(eigVal, 
			-(a + c),
			a * c - b * b))
	{
		if (eigVal[0] == eigVal[1])
		{
			for (int i = 0; i < 2; i++)
			{
				eigVec[i].makeunit(i);
			}
		}
		else
		{
			for (int i = 0; i < 2; i++)
			{
				eigVec[i][0] = eigVal[i] - c;
				eigVec[i][1] = b;

				double li = length(eigVec[i]);
				if (li > FLT_EPSILON)
				{
					eigVec[i] /= li;
				}
				else 
				{
					return false;
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

inline bool SymEigenDecomp(const Vec<6,double> &m, 
						   double eigVal[3], 
						   Vec3double eigVec[3])
//eigen decompose symmetric matrix
//[a, b, c; 
// b, d, e; 
// c, e, f]
//the solution comes from Mathematica
{
	const double a = m[0];
	const double b = m[1];
	const double c = m[2];
	const double d = m[3];
	const double e = m[4];
	const double f = m[5];

	if (SolveRealCubicRoots(eigVal, 
			-(a + d + f), 
			a * d + a * f + d * f - b * b - c * c - e * e, 
			c * c * d + a * e * e + b * b * f - a * d * f - 2 * b * c * e ))
	{
		if (eigVal[0] == eigVal[1] && eigVal[1] == eigVal[2])
		{
			for ( int i = 0; i < 3; i ++ )
			{
				eigVec[i].makeunit(i);
			}
		}
		else if (eigVal[0] != eigVal[1] && eigVal[1] != eigVal[2] && eigVal[2] != eigVal[0])
		{
			for ( int i = 0; i < 3; i ++ )
			{
				double nom = c * e + b * ( eigVal[i] - f );
				double den = b * e + c * ( eigVal[i] - d );

				eigVec[i][0] = ( eigVal[i] - f ) * den - e * nom;
				eigVec[i][1] = nom * c;
				eigVec[i][2] = den * c;

				double li = length(eigVec[i]);

				if (li < FLT_EPSILON) return false;

				eigVec[i] /= li;
			}		
		}
		else
		{
			int i, j, k;

			for (i = 0; i < 3; i++)
			{
				j = (i + 1) % 3;
				k = (i + 2) % 3;
				if (eigVal[j] == eigVal[k])
				{
					break;
				}
			}
			if (i == 3)
			{
				return false;
			}

			//eigVal[i] is different from eigVal[j] == eigVal[k];
			//m - eigVal[j] * iden is rank ONE
			//the row space is the eigen vector for eigVal[i]
			
			if ( a != eigVal[j] || b != 0.0 || c != 0.0 )
			{
				eigVec[i][0] = a - eigVal[j];
				eigVec[i][1] = b;
				eigVec[i][2] = c;
			}
			else if ( b != 0.0 || d != eigVal[j] || e != 0.0 )
			{
				eigVec[i][0] = b;
				eigVec[i][1] = d - eigVal[j];;
				eigVec[i][2] = e;
			}
			else if ( c != 0.0 || e != 0.0 || f != eigVal[j] )
			{
				eigVec[i][0] = c;
				eigVec[i][1] = e;
				eigVec[i][2] = f - eigVal[j];
			}
			else 
			{
				return false;
			}
			
			double li = length(eigVec[i]);

			if (li < FLT_EPSILON) return false;

			eigVec[i] /= li;

			int h;
			for (h = 0; h < 3; h++)
			{
				if (eigVec[i][h] != 0.0)
				{
					break;
				}
			}
			if (h == 3)
			{
				return false;
			}

			int m = (h + 1) % 3;
			int n = (h + 2) % 3;

			eigVec[j][h] = eigVec[i][m];
			eigVec[j][m] = -eigVec[i][h];
			eigVec[j][n] = 0.0;

			double lj = length(eigVec[j]);

			if (lj < FLT_EPSILON) return false;

			eigVec[j] /= lj;

			eigVec[k] = cross(eigVec[i], eigVec[j]);
		}
		return true;
	}
	else
	{
		return false;
	}
}

//inline void test()
//{
	/*
	double y[3] = {1, 1, 2};
	double a2 = -(y[0] + y[1] + y[2]);
	double a1 = y[0] * y[1] + y[0] * y[2] + y[1] * y[2];
	double a0 = -(y[0] * y[1] * y[2]);
	double z[3];
	SolveRealCubicRoots(z, a2, a1, a0);
	*/

	/*
	Vec<6, double> m;
	m[0] = 1.0; 
	m[1] = 2.0;
	m[2] = 3.0;
	m[3] = 4.0;
	m[4] = 5.0;
	m[5] = 6.0;

	m[0] = 1.0;
	m[1] = 0.0;
	m[2] = 0.0;
	m[3] = 2.0;
	m[4] = 0.0;
	m[5] = 2.0;

	m[0] = -0.2020;
	m[1] = -0.1660;
	m[2] = 0.2991;
	m[3] = 0.0970;
	m[4] = 0.1331;
	m[5] = -0.0689;

	double eigVal[3];
	Vec3double eigVec[3];

	SymEigenDecomp(m, eigVal, eigVec);

	[0.66489771172976242, -0.19447387173120637, 0.72117331214514591;
	0.73696468288081063, 0.32802790610451060, -0.59099978765061889;
	-0.12163095467324529, 0.92443366774303071, 0.36142537930870744]
	*/

	/*
	Vec3double m;
	m[0] = 2.0;
	m[1] = 0.0;
	m[2] = 2.0;

	double eigVal[2];
	Vec2double eigVec[2];
	SymEigenDecomp(m, eigVal, eigVec);
	*/
//}

#endif
