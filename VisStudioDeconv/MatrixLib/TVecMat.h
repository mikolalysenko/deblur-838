#ifndef TVecMat_h
#define TVecMat_h

#include <math.h>

#include <iostream>
#include <vector>
using namespace std;
using std::vector;

// some auxiliary matrix and vector classes

// vector definition;

template <int N, class floating> 
class Vec
{
protected:

	floating v[N];

public:

	inline static Vec <N,floating> zero(void);
	inline static Vec <N,floating> unit(int i);

	inline Vec (void) {}
	inline Vec (const floating v[N]);
	inline Vec (const Vec <N,floating> &v);

	inline bool iszero(void) const;
	inline bool isconstant(floating f) const;
	inline void makezero(void);
	inline void makeunit(int i);

	inline floating &operator [] (int i)
	{ return v[i]; }

	inline const floating &operator [] (int i) const 
	{ return v[i]; }

	inline const floating *buffer(void) const 
	{ return v; }
	inline floating *buffer(void) 
	{ return v; }

	inline bool operator == (const Vec <N, floating> &vec) const;
	inline bool operator != (const Vec <N, floating> &vec) const
	{ return ! (*this == vec); }

	inline Vec <N, floating>& operator = (const Vec <N, floating> &vec);	
	inline Vec <N, floating>& operator = (const floating &f);

	inline Vec <N, floating>& operator += (const Vec <N, floating> &vec);
	inline Vec <N, floating> operator + (const Vec <N, floating> &vec) const
	{ return Vec <N,floating> (*this) += vec; }
		
	inline Vec <N, floating>& operator -= (const Vec <N, floating> &vec);
	inline Vec <N, floating> operator - (const Vec <N, floating> &vec) const
	{ return Vec <N,floating> (*this) -= vec; }

	inline Vec <N, floating>& operator *= (floating s);
	inline Vec <N, floating> operator * (floating s) const
	{ return Vec <N,floating> (*this) *= s; }

	inline Vec <N, floating>& operator /= (floating s)
	{ return (*this)*=((floating)1/s); }
	inline Vec <N, floating> operator / (floating s) const
	{ return (*this)*((floating)1/s); }
};

template <int N, class floating>
Vec <N, floating> operator - (const Vec <N, floating>&v);

template <int N, class floating> 
Vec <N, floating> operator * (floating s, const Vec<N,floating> &v)
{ return Vec <N,floating> (v) *= s; }

template <int N, class floating>
floating inner(const Vec <N,floating> &a, const Vec <N,floating> &b);

template <int N, class floating>
floating length(const Vec <N,floating> &v)
{ return (floating)sqrt(inner(v,v)); }

template <class floating> 
Vec<3, floating> cross(const Vec<3,floating> &a, const Vec<3,floating> &b);

template <int N, class floating>
istream& operator >> (istream &is, Vec<N, floating> &m);

template <int N, class floating>
ostream& operator << (ostream &os, const Vec<N, floating> &m);

#include "TVec.inl"

typedef Vec <2,unsigned char>	Vec2byte;
typedef Vec <2,int>				Vec2int;
typedef Vec <2,float>			Vec2float;
typedef Vec <2,double>			Vec2double;

typedef Vec <3,unsigned char>	Vec3byte;
typedef Vec <3,int>				Vec3int;
typedef Vec <3,float>			Vec3float;
typedef Vec <3,double>			Vec3double;

typedef Vec <4,unsigned char>	Vec4byte;
typedef Vec <4,int>				Vec4int;
typedef Vec <4,float>			Vec4float;
typedef Vec <4,double>			Vec4double;

// matrix definition;

template <int M, int N, class floating>
class Mat
{
public:

	Vec <N,floating> m[M];

public:

	static Mat <M,N,floating> zero(void);

	inline Mat (void) {}
	inline Mat (const floating v[M][N]);

	inline Mat (const Mat <M,N,floating> &v);

	inline Vec <N,floating> &operator [] (int i)
	{ return m[i]; }

	inline const Vec <N,floating> &operator [] (int i) const 
	{ return m[i]; }

	inline Mat <M, N, floating> &operator = (const Mat <M, N, floating> &mat);
	inline Mat <M, N, floating> &operator = (const floating &f);

	inline void makezero(void) 
	{ for (int i = 0; i < M; i++) m[i].makezero(); }

	inline Mat <M, N, floating> &operator += (const Mat <M, N, floating> &mat);
	inline Mat <M, N, floating> operator + (const Mat <M, N, floating> &mat) const
	{ return Mat <M,N,floating> (*this) += mat; }

	inline Mat <M, N, floating> &operator -= (const Mat <M, N, floating> &mat);
	inline Mat <M, N, floating> operator - (const Mat <M, N, floating> &mat) const
	{ return Mat <M,N,floating> (*this) -= mat; }

	inline Mat <M, N, floating>& operator *= (floating s);
	inline Mat <M, N, floating> operator * (floating s) const
	{ return Mat <M,N,floating> (*this) *= s; }	

	inline Vec <M, floating> operator * (const Vec <N, floating> &vec) const;	

	inline Mat <M, N, floating>& operator /= (floating s)
	{ return (*this)*=(1.0/s); }
	inline Mat <M, N, floating> operator / (floating s) const
	{ return (*this)*(1.0/s); }	
};

template <int M, int N, class floating>
Mat <M, N, floating> operator - (const Mat <M,N, floating>&m);

template <int M, int N, class floating>
Mat <M, N, floating> operator *( floating s, const Mat<M, N, floating>& m)
{ return Mat <M, N, floating> (m) *= s; }

template <int M, int N, class floating>
Vec <N, floating> operator *( const Vec <M,floating> &v, const Mat<M, N, floating>& m);

template <int M, int N, class floating>
Mat <M, N, floating> outer(const Vec <M,floating> &a, const Vec <N,floating> &b);

template <int L, int M, int N, class floating>
Mat <L,N,floating> operator * (const Mat <L,M,floating> &a, const Mat <M,N,floating> &b);

template <int M, int N, class floating>
Mat<N,M,floating> transpose(const Mat<M,N,floating> &m);

template <int M, int N, class floating>
istream& operator >> (istream &is, Mat<M, N, floating> &m);

template <int M, int N, class floating>
ostream& operator << (ostream &os, const Mat<M, N, floating> &m);

#include "TMat.inl"

// square matrix definition;

template <int N, class floating>
class SqMat : public Mat<N,N,floating>
{
public:
	inline static SqMat <N,floating> iden(void);
	inline static SqMat <N,floating> zero(void);

	inline SqMat (void) {}
	inline SqMat ( const floating mat[N][N] );

	inline SqMat ( const SqMat<N,floating> & mat );

	inline Vec <N,floating> &operator [] (int i)
	{ return Mat<N,N,floating>::m[i]; }

	inline const Vec <N,floating> &operator [] (int i) const 
	{ return Mat<N,N,floating>::m[i]; }

	inline SqMat <N, floating> &operator = (const SqMat <N, floating> &mat);
	inline SqMat <N, floating> &operator = (const floating &f);

	inline void makeiden(void) 
	{ for (int i = 0; i < N; i++) Mat<N,N,floating>::m[i].makeunit(i); }
	inline void makezero(void) 
	{ for (int i = 0; i < N; i++) Mat<N,N,floating>::m[i].makezero(); }
	inline void makesymm(const floating *v);

	inline SqMat <N, floating> &operator += (const SqMat <N, floating> &mat);
	inline SqMat <N, floating> operator + (const SqMat <N, floating> &mat) const
	{ return SqMat <N,floating> (*this) += mat; }	
	
	inline SqMat <N, floating> &operator -= (const SqMat <N, floating> &mat);
	inline SqMat <N, floating> operator - (const SqMat <N, floating> &mat) const
	{ return SqMat <N,floating> (*this) -= mat; }
		
	inline SqMat <N, floating>& operator *= (floating s);
	inline SqMat <N, floating> operator * (floating s) const
	{ return SqMat <N,floating> (*this) *= s; }

	inline Vec <N, floating> operator * (const Vec <N, floating> &vec) const;

	inline SqMat <N, floating>& operator /= (floating s)
	{ return (*this)*=(1.0/s); }
	inline SqMat <N, floating> operator / (floating s) const
	{ return (*this)*(1.0/s); }

};

template <int N, class floating>
SqMat <N, floating> operator - (const SqMat <N, floating>&m);

template <int N, class floating>
SqMat <N, floating> operator *( floating s, const SqMat<N, floating>& m)
{ return SqMat <N, floating> (m) *= s; }

template <int N, class floating>
Vec <N, floating> operator *( const Vec <N,floating> &v, const SqMat<N, floating>& m);

template <int N, class floating>
SqMat <N, floating> outer(const Vec <N,floating> &a, const Vec <N,floating> &b);

template <int N, class floating>
SqMat <N,floating> operator * (const SqMat <N,floating> &a, const SqMat <N,floating> &b);

template <int M, int N, class floating>
Mat <M,N,floating> operator * (const Mat <M,N,floating> &a, const SqMat <N,floating> &b);

template <int M, int N, class floating>
Mat <M,N,floating> operator * (const SqMat <M,floating> &a, const Mat <M,N,floating> &b);

template <int N, class floating> 
floating trace(const SqMat<N,floating> &m);

template <int N, class floating>
SqMat<N,floating> transpose(const SqMat<N,floating> &m);

template <int N, class floating>
floating determinant(const SqMat<N,floating> &m);

template <int N, class floating>
SqMat<N,floating> inverse(const SqMat<N,floating> &m);

template <int N, class floating> 
Vec<N, floating> solve(const SqMat<N,floating> &A, const Vec<N,floating> &b);

template <int N, class floating> 
Vec<N, floating> solve(const SqMat<N,floating> &A, const Vec<N,floating> &b, int M);

template <int N, class floating> 
void upperright2lowerleft(SqMat<N,floating> &M);

template <int N, class floating> 
void lowerleft2upperright(SqMat<N,floating> &M);

template <class floating> 
SqMat<3, floating> skew(const Vec<3,floating> &n); //skew matrix from vector n;

template <class floating> 
SqMat<3, floating> rotate_around(const Vec<3,floating> &axis, double c, double s); //angle is in rad

template <class floating> 
SqMat<3, floating> rotate_around(const Vec<3,floating> &axis, floating angle) //angle is in rad
{ return rotate_around(axis, cos(angle), sin(angle)); }

template <class floating>
SqMat<3, floating> rotate_between(const Vec<3,floating> &a, const Vec<3,floating> &b); //from a to b

template <int N, class floating>
istream& operator >> (istream &is, SqMat<N, floating> &m);

template <int N, class floating>
ostream& operator << (ostream &os, const SqMat<N, floating> &m);

#include "TSqMat.inl"

typedef SqMat <2,float> SqMat2float;
typedef SqMat <2,double> SqMat2double;

typedef SqMat <3,float> SqMat3float;
typedef SqMat <3,double> SqMat3double;

typedef SqMat <4,float> SqMat4float;
typedef SqMat <4,double> SqMat4double;

template <class floating>
Vec<2, floating> Homo3Multiply(const SqMat<3, floating> H, const Vec<2, floating> &p)
{
	floating x = H[0][0] * p[0] + H[0][1] * p[1] + H[0][2];
	floating y = H[1][0] * p[0] + H[1][1] * p[1] + H[1][2];
	floating z = H[2][0] * p[0] + H[2][1] * p[1] + H[2][2];
	floating iz = (floating)1 / z;
	Vec<2, floating> res;
	res[0] = x * iz;
	res[1] = y * iz;
	return res;
}

#endif
