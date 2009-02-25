#ifndef PVecMat_h
#define PVecMat_h

#include <valarray>

using std::valarray;

template <typename floating> 
class Varray1D : public valarray<floating>
{
public:
	Varray1D():valarray<floating>() {};
	Varray1D(size_t n):valarray<floating>(n) {};
	Varray1D(const floating &val, size_t n):valarray<floating>(val, n)) {};
	Varray1D(const floating *p, size_t n):valarray<floating>(p, n) {};

	inline floating &operator () (int i)
	{ return (*this)[i]; }
	inline const floating &operator () (int i) const
	{ return (*this)[i]; }

	inline const floating *ptr(void) const 
	{ return &((*this)[0]); }
	inline floating *ptr(void) 
	{ return &((*this)[0]); }	
};

template <typename floating>
class Varray2D : public Varray1D<floating>
{
protected:
	size_t _nRows, _nCols;

public: 
	Varray2D():Varray1D<floating>() 
	{ _nRows = _nCols = 0; }
	Varray2D(size_t h, size_t w):Varray1D<floating>(h*w) 
	{ _nRows = _nCols = 0; }
	Varray2D(const floating &val, size_t h, size_t w):Varray1D<floating>(val, h*w))
	{ _nRows = _nCols = 0; }
	Varray2D(const floating *p, size_t h, size_t w):Varray1D<floating>(p, h*w)
	{ _nRows = _nCols = 0; }

	void resize(size_t h, size_t w, const floating& c = T())
	{ _nRows = h; _nCols = w; Varray1D<floating>::resize(h*w, c) };

	inline floating &operator () (int i, int j)
	{ return (*this)[i*_nCols+j]; }
	inline const floating &operator () (int i, int j) const
	{ return (*this)[i*_nCols+j]; }
};

/*


	template <int N, class floating>
floating inner(const Vec <N,floating> &a, const Vec <N,floating> &b);

template <int N, class floating>
floating length(const Vec <N,floating> &v)
{ return (floating)sqrt(inner(v,v)); }


template <int M, int N, class floating>
Vec <N, floating> operator *( const Vec <M,floating> &v, const Mat<M, N, floating>& m);

template <int M, int N, class floating>
Mat <M, N, floating> outer(const Vec <M,floating> &a, const Vec <N,floating> &b);

template <int L, int M, int N, class floating>
Mat <L,N,floating> operator * (const Mat <L,M,floating> &a, const Mat <M,N,floating> &b);

template <int M, int N, class floating>
Mat<N,M,floating> transpose(const Mat<M,N,floating> &m);


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
void move(floating *pnt, const double *start, const floating *direction, floating scaler, size_t n)
// pnt[] += start[] + scaler * direction[]
{
	floating *p = pnt;
	const floating *s = start;
	const floating *d = direction;
	
	for (size_t i = 0; i < n; i++)
	{
		*p = (*s) + (*d) * scaler;
		p ++;
		s ++;
		d ++;
	}
}

template <class floating> 
void move_assign(floating *pnt, const floating *direction, floating scaler, size_t n)
// pnt[] += scaler * direction[]
{
	floating *p = pnt;
	const floating *d = direction;
	
	for (size_t i = 0; i < n; i++)
	{
		*p += (*d) * scaler;
		p ++;
		d ++;
	}
}

template <class floating>
floating inner(const floating *a, const floating *b, size_t n)
{
	const floating *pa = a;
	const floating *pb = b;
	floating sum = (floating)0;

	for (size_t i = 0; i < n; i++)
	{
		sum += (*pa) * (*pb);
		pa ++;
		pb ++;
	}
	return sum;
}

template <class floating> 
void multiply(floating *lhs, const floating **rhs1, const floating *rhs2, size_t m, size_t n)
// lhs[] = rhs1[][] * rhs2[]
{
	floating *pl = lhs;
	const floating ** pr1 = rhs1;

	for (size_t i = 0; i < m; i++)
	{
		*pl = inner(*pr1, rhs2, n);
		pl ++;
		pr1 ++;
	}
}

template <class floating> 
void multiply(floating *lhs, const floating *rhs1, const floating **rhs2, size_t m, size_t n)
// lhs[] = rhs1[] * rhs2[][]
{
	floating *pl = lhs;
	
	for (size_t j = 0; j < n; j++)
	{
		*pl = (floating)0;
		const floating *pr1 = rhs1;
		const floating **pr2 = rhs2;
		for (size_t i = 0; i < m; i++)
		{
			*pl += (*pr1) * (*pr2)[j];
			pr1 ++;
			pr2 ++;
		}
		pl ++;		
	}
}

template <class floating> 
void multiply(floating **lhs, const floating ** rhs1, const floating ** rhs2, size_t l, size_t m, size_t n)
// lhs[][] = rhs1[][] * rhs2[][]
{
	for (size_t i = 0; i<l; i++)
	{
		multiply(lhs[i], rhs1[i], rhs2, m, n);
	}
}

template <class floating> 
void transpose(floating **lhs, const floating **rhs, size_t m, size_t n)
{
	for (size_t i = 0; i<m; i++)
	{
		for (size_t j = 0; j<n; j++)
		{
			lhs[j][i] = rhs[i][j];
		}
	}
}

template <class floating> 
void transpose_assign(floating **mat, size_t n)
{
	for (size_t i = 1; i<n; i++)
	{
		for (size_t j = 0; j<i; j++)
		{
			//swap mat[j][i] and mat[i][j];
			floating tmp = mat[i][j];
			mat[i][j] = mat[j][i];
			mat[j][i] = tmp;
		}
	}
}

template <class floating>
floating norm_inf(const floating *vec, size_t n)
{
	floating res = (floating)fabs((double)(*vec));
	vec++;
	for (size_t i = 1; i < n; i++)
	{
		floating r = (floating)fabs((double)(*vec));
		res = max(r, res);
		vec++;
	}
	return res;
}

template <class floating> 
int move_assign_count(floating *pnt, const floating *direction, floating scaler, size_t n, floating res)
// pnt[] += scaler * direction[]
// count movement larger than res
{
	floating *p = pnt;
	const floating *d = direction;
	int count = 0;
	
	for (size_t i = 0; i < n; i++)
	{
		floating mv = (*d) * scaler;
		*p += mv;
		if ((floating)fabs(mv) > res) count ++;
		p ++;
		d ++;
	}

	return count;
}

template <class floating>
void make_symm_iden(floating *vec, size_t n)
{
	memset(vec, 0, sizeof(floating)*(n*(n+1)/2));

	for (size_t i = 0; i < n; i++)
	{
		*vec = 1.0;
		vec += (n - i);
	}
}

template <class floating>
size_t count_large_elements(const floating *vec, size_t n, floating t)
{
	size_t count = 0;
	for (size_t i = 0; i < n; i++)
	{
		count += ((floating)fabs((double)(*vec)) > t);
		vec ++;
	}
	return count;
}

template <class floating>
void output_vector(ostream &os, const floating *vec, size_t n, const char *delimiter = "\n")
{
	for (size_t i = 0; i<n; i++)
	{
		os << vec[i] << delimiter;
	}
}

template <class floating> 
void output_matrix(ostream &os, const floating **mat, size_t m, size_t n, const char *row_delim = "\n", const char *col_delim = "\t")
{
	for (size_t i = 0; i<m; i++)
	{
		output_vector(os, mat[i], n, col_delim);
		os << row_delim;
	}
}
*/

#endif
