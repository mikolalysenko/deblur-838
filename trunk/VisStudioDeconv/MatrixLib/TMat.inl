#ifndef TMat_inl
#define TMat_inl

template <int M, int N, class floating> 
Mat <M,N,floating> Mat<M,N,floating>::zero(void)
{
	Mat <M,N,floating> z;
	int d;
	for (d=0;d<M;d++)
	{
		z[d] = Vec<N,floating>::zero();
	}
	return z;
}

template <int M, int N, class floating> 
Mat <M, N, floating> &Mat<M, N, floating>::operator = (const Mat <M, N, floating> &mat)
{	
	int d;
	for (d=0;d<M;d++)
	{
		m[d] = mat[d];
	}
	return *this;
}

template <int M, int N, class floating> 
Mat <M, N, floating> :: Mat( const floating mat[M][N] )
{	
	for (int j=0; j<M; j++)
	{
		for (int i=0; i<N; i++)
		{
			m[j][i] = mat[j][i];
		}
	}
}

template <int M, int N, class floating> 
Mat <M, N, floating> :: Mat( const Mat<M,N,floating> & mat )
{
	int d;
	for (d=0; d<M; d++)
	{
		m[d] = mat[d];
	}
}

template <int M, int N, class floating> 
Mat <M, N, floating> &Mat<M, N, floating>::operator = (const floating &f)
{
	int d;
	for (d=0;d<M;d++)
	{
		m[d] = f;
	}
	return *this;
}

template <int M, int N, class floating> 
Mat <M, N, floating> &Mat<M, N, floating>::operator += (const Mat <M, N, floating> &mat)
{	
	int d;
	for (d=0;d<M;d++)
	{
		m[d] += mat[d];
	}
	return *this;
}

template <int M, int N, class floating> 
Mat <M, N, floating> &Mat<M, N, floating>::operator -= (const Mat <M, N, floating> &mat)
{	
	int d;
	for (d=0;d<M;d++)
	{
		m[d] -= mat[d];
	}
	return *this;
}

template <int M, int N, class floating>
Mat <M, N, floating> &Mat<M, N, floating>::operator *= (floating s)
{	
	int d;
	for (d=0;d<M;d++)
	{
		m[d] *= s;
	}
	return *this;
}

template <int M, int N, class floating>
Vec <M, floating> Mat<M, N, floating>::operator * (const Vec <N, floating> &vec) const
{
	Vec <M, floating> prod;
	int d;
	for (d=0;d<M;d++)
	{
		prod[d] = inner(m[d],vec);
	}
	return prod;
}

template <int M, int N, class floating>
Mat <M, N, floating> operator - (const Mat <M,N, floating>&m)
{
	Mat <M,N, floating> n;
	int d;
	for (d=0;d<M;d++)
	{
		n[d]=-m[d];
	}
	return n;
}

template <int M, int N, class floating>
Vec <N, floating> operator *( const Vec <M,floating> &v, const Mat<M, N, floating>& m)
{	
	Vec <N, floating> prod = Vec <N, floating>::zero();

	int d;
	for (d=0;d<M;d++)
	{
		prod+=v[d]*m[d];
	}

	return prod;
}

template <int M, int N, class floating>
Mat <M, N, floating> outer(const Vec <M,floating> &a, const Vec <N,floating> &b)
{
	Mat<M,N,floating> m;
	int d;
	for (d=0;d<M;d++)
	{
		m[d] = a[d]*b;
	}
	return m;
}

template <int L, int M, int N, class floating>
Mat <L,N,floating> operator * (const Mat <L,M,floating> &a, const Mat <M,N,floating> &b)
{
	Mat <L,N,floating> prod;
	int d;
	for (d=0;d<L;d++)
	{
		prod[d] = a[d] * b;
	}
	return prod;
}

template <int M, int N, class floating>
Mat<N,M,floating> transpose(const Mat<M,N,floating> &m)
{
	Mat<N,M,floating> mt;
	int i,j;
	for (j=0;j<N;j++)
	{
		for (i=0;i<M;i++)
		{
			mt[j][i] = m[i][j];
		}
	}
	return mt;
}

template <int M, int N, class floating>
istream& operator >> (istream &is, Mat<M, N, floating> &m)
{
	int j;

	for (j=0;j<M;j++)
	{
		is>>m[j];
	}
	return is;
}

template <int M, int N, class floating>
ostream& operator << (ostream &os, const Mat<M, N, floating> &m)
{
	int j;

	for (j=0;j<M;j++)
	{
		os<<m[j];
	}
	return os;
}

#endif
