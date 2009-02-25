#ifndef TSqMat_inl
#define TSqMat_inl

template <int N, class floating> 
SqMat <N,floating> SqMat<N,floating>::iden(void)
{
	SqMat <N,floating> id;
	int d;
	for (d=0;d<N;d++)
	{
		id[d] = Vec<N,floating>::unit(d);
	}
	return id;
}

template <int N, class floating> 
SqMat <N,floating> SqMat<N,floating>::zero(void)
{
	SqMat <N,floating> id;
	int d;
	for (d=0;d<N;d++)
	{
		id[d] = Vec<N,floating>::zero();
	}
	return id;
}

template <int N, class floating> 
SqMat <N, floating>::SqMat( const floating mat[N][N] )
{	
	for (int j=0; j<N; j++)
	{
		for (int i=0; i<N; i++)
		{
			Mat<N,N,floating>::m[j][i] = mat[j][i];
		}
	}
}

template <int N, class floating> 
SqMat <N, floating> :: SqMat( const SqMat<N,floating> & mat )
{
	int d;
	for (d=0; d<N; d++)
	{
		Mat<N,N,floating>::m[d] = mat[d];
	}
}

template <int N, class floating> 
SqMat <N, floating> &SqMat<N, floating>::operator = (const SqMat <N, floating> &mat)
{	
	int d;
	for (d=0;d<N;d++)
	{
		Mat<N,N,floating>::m[d] = mat[d];
	}
	return *this;
}

template <int N, class floating> 
SqMat <N, floating> &SqMat<N, floating>::operator = (const floating &f)
{	
	int d;
	for (d=0;d<N;d++)
	{
		Mat<N,N,floating>::m[d] = f;
	}
	return *this;
}

template <int N, class floating>
void SqMat<N, floating>::makesymm(const floating *v)
{
	int count = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = i; j < N; j++)
		{			
			Mat<N,N,floating>::m[i][j] = Mat<N,N,floating>::m[j][i] = v[count];
			count ++;
		}
	}
}

template <int N, class floating> 
SqMat <N, floating> &SqMat<N, floating>::operator += (const SqMat <N, floating> &mat)
{	
	int d;
	for (d=0;d<N;d++)
	{
		Mat<N,N,floating>::m[d] += mat[d];
	}
	return *this;
}

template <int N, class floating> 
SqMat <N, floating> &SqMat<N, floating>::operator -= (const SqMat <N, floating> &mat)
{	
	int d;
	for (d=0;d<N;d++)
	{
		Mat<N,N,floating>::m[d] -= mat[d];
	}
	return *this;
}

template <int N, class floating>
SqMat <N, floating> &SqMat<N, floating>::operator *= (floating s)
{	
	int d;
	for (d=0;d<N;d++)
	{
		Mat<N,N,floating>::m[d] *= s;
	}
	return *this;
}

template <int N, class floating>
Vec <N, floating> SqMat<N, floating>::operator * (const Vec <N, floating> &vec) const
{
	Vec <N, floating> prod;
	int d;
	for (d=0;d<N;d++)
	{
		prod[d] = inner(Mat<N,N,floating>::m[d],vec);
	}
	return prod;
}

template <int N, class floating>
SqMat <N, floating> operator - (const SqMat <N, floating>&m)
{
	SqMat <N, floating> n;
	int d;
	for (d=0;d<N;d++)
	{
		n[d]=-m[d];
	}
	return n;
}

template <int N, class floating>
Vec <N, floating> operator *( const Vec <N,floating> &v, const SqMat<N, floating>& m)
{	
	Vec <N, floating> prod = Vec <N, floating>::zero();

	int d;
	for (d=0;d<N;d++)
	{
		prod+=v[d]*m[d];
	}

	return prod;
}

template <int N, class floating>
SqMat <N, floating> outer(const Vec <N,floating> &a, const Vec <N,floating> &b)
{
	SqMat<N,floating> m;
	int d;
	for (d=0;d<N;d++)
	{
		m[d] = a[d]*b;
	}
	return m;
}

template <int N, class floating>
SqMat <N,floating> operator * (const SqMat <N,floating> &a, const SqMat <N,floating> &b)
{
	SqMat <N,floating> prod;
	int d;
	for (d=0;d<N;d++)
	{
		prod[d] = a[d] * b;
	}
	return prod;
}

template <int M, int N, class floating>
Mat <M,N,floating> operator * (const Mat <M,N,floating> &a, const SqMat <N,floating> &b)
{
	Mat <M,N,floating> prod;
	int d;
	for (d=0;d<M;d++)
	{
		prod[d] = a[d] * b;
	}
	return prod;
}

template <int M, int N, class floating>
Mat <M,N,floating> operator * (const SqMat <M,floating> &a, const Mat <M,N,floating> &b)
{
	Mat <M,N,floating> prod;
	int d;
	for (d=0;d<M;d++)
	{
		prod[d] = a[d] * b;
	}
	return prod;
}

template <int N, class floating> 
floating trace(const SqMat<N,floating> &m)
{
	floating sum = 0;
	int d;
	for (d=0;d<N;d++)
	{ 
		sum+=m[d][d];
	}
	return sum;
}

template <int N, class floating>
SqMat<N,floating> transpose(const SqMat<N,floating> &m)
{
	SqMat<N,floating> mt;
	int i,j;
	for (j=0;j<N;j++)
	{
		for (i=0;i<N;i++)
		{
			mt[j][i] = m[i][j];
		}
	}
	return mt;
}

template <int N, class floating>
floating determinant(const SqMat<N,floating> &m)
{
		// Matrix inverse  (use un-pivoted Gaussian elimination for now)
	SqMat<N,floating> M0(m);
	floating det = 1;
    
    // Forward elimination
    int i;
    for (i = 0; i < N; i++)
    {
		int j;

		det *= M0[i][i];

        // Normalize the row		
        floating row_inv = (floating)1 / M0[i][i];

		M0[i][i] = 1.0;

		for (j = i+1; j<N; j++)
		{
			M0[i][j] *= row_inv;
		}
		
        // Subtract from lower ones

        for (int k = i+1; k < N; k++)
        {
            floating mult = M0[k][i];

			M0[k][i] = (floating)0;

			for (j=i+1; j<N; j++)
			{
				M0[k][j] -= mult * M0[i][j];
			}
        }
    }
	return det;
}

template <int N, class floating>
SqMat<N,floating> inverse(const SqMat<N,floating> &m)
{
	// Matrix inverse  (use un-pivoted Gaussian elimination for now)
	SqMat<N,floating> M0(m);
	SqMat<N,floating> M1 = SqMat<N,floating>::iden();
    
    // Forward elimination
    int i;
    for (i = 0; i < N; i++)
    {
		int j;

        // Normalize the row

        floating row_inv = (floating)1 / M0[i][i];

		M0[i][i] = 1.0;
		M1[i][i] = row_inv;

		for (j = i+1; j<N; j++)
		{
			M0[i][j] *= row_inv;
		}

		for (j = 0; j<i; j++)
		{			
            M1[i][j] *= row_inv;
		}
		
        // Subtract from lower ones

        for (int k = i+1; k < N; k++)
        {
            floating mult = M0[k][i];

			M0[k][i] = (floating)0;

			for (j=i+1; j<N; j++)
			{
				M0[k][j] -= mult * M0[i][j];
			}

			for (j=0; j<=i; j++)
			{
				M1[k][j] -= mult * M1[i][j];				
			}
        }
    }

    // Backward elimination
    for (i = N-1; i > 0; i--)
    {
        // Subtract from upper ones
        for (int k = i-1; k >= 0; k--)
        {
            floating mult = M0[k][i];
            for (int j = 0; j < N; j++)
            {
                M1[k][j] -= mult * M1[i][j];
            }
        }
    }
    return M1;
}

template <int N, class floating> 
Vec<N, floating> solve(const SqMat<N,floating> &A0, const Vec<N,floating> &b0)
{
	SqMat<N, floating> A(A0);
	Vec<N, floating> b(b0);

	// Forward elimination
    int i;
    for (i = 0; i < N; i++)
    {
		int j;

        // Normalize the row

        floating row_inv = (floating)1 / A[i][i];

		A[i][i] = 1.0;
		b[i] *= row_inv;

		for (j = i+1; j<N; j++)
		{
			A[i][j] *= row_inv;
		}
		
        // Subtract from lower ones

        for (int k = i+1; k < N; k++)
        {
            floating mult = A[k][i];

			A[k][i] = (floating)0;

			for (j=i+1; j<N; j++)
			{
				A[k][j] -= mult * A[i][j];
			}

			b[k] -= mult*b[i];
        }
    }

    // Backward elimination
    for (i = N-1; i > 0; i--)
    {
        // Subtract from upper ones
        for (int k = i-1; k >= 0; k--)
        {
            floating mult = A[k][i];
			b[k] -= mult * b[i];
        }
    }

	return b;
}

template <int N, class floating> 
Vec<N, floating> solve(const SqMat<N,floating> &A0, const Vec<N,floating> &b0, int M)
{
	SqMat<N, floating> A(A0);	
	Vec<N, floating> b(b0);

	int i;
	for (i = M; i < N; i++)
	{
		b[i] = (floating)0;
	}

	// Forward elimination    
    for (i = 0; i < M; i++)
    {
		int j;

        // Normalize the row

        floating row_inv = (floating)1 / A[i][i];

		A[i][i] = 1.0;
		b[i] *= row_inv;

		for (j = i+1; j<M; j++)
		{
			A[i][j] *= row_inv;
		}
		
        // Subtract from lower ones

        for (int k = i+1; k < M; k++)
        {
            floating mult = A[k][i];

			A[k][i] = (floating)0;

			for (j=i+1; j<M; j++)
			{
				A[k][j] -= mult * A[i][j];
			}

			b[k] -= mult*b[i];
        }
    }

    // Backward elimination
    for (i = M-1; i > 0; i--)
    {
        // Subtract from upper ones
        for (int k = i-1; k >= 0; k--)
        {
            floating mult = A[k][i];
			b[k] -= mult * b[i];
        }
    }	

	return b;
}

template <class floating> 
SqMat<3, floating> skew(const Vec<3,floating> &n) //skew matrix from vector n;
{
	SqMat <3, floating> skew;
	//[0	-nz	 ny]
	//[nz   0	-nx]
	//[-ny  nx   0]

	skew[0][0] = skew[1][1] = skew[2][2] = (floating)0;
	skew[0][1] = -n[2];
	skew[1][0] = n[2];
	skew[0][2] = n[1];
	skew[2][0] = -n[1];
	skew[1][2] = -n[0];
	skew[2][1] = n[0];

	return skew;
}

template <class floating> 
SqMat<3, floating> rotate_around(const Vec<3,floating> &axis, double c, double s)
{
	Vec<3, floating> naxis = axis / length(axis);	
	//rodrigues's formula:
	return c * SqMat<3,floating>::iden() + (1-c) * outer<3,floating>(naxis,naxis) + s * skew(naxis);
}

template <class floating>
SqMat<3, floating> rotate_between(const Vec<3,floating> &a, const Vec<3,floating> &b) //from a to b
{
	Vec <3,floating> na = a/length(a);
	Vec <3,floating> nb = b/length(b);

	double c = inner(na,nb);
	double s = sqrt(1-c*c);

	return rotate_around(cross(a,b), c, s);
}

template <int N, class floating>
istream& operator >> (istream &is, SqMat<N, floating> &m)
{
	int j;

	for (j=0;j<N;j++)
	{
		is>>m[j];
	}
	return is;
}

template <int N, class floating>
ostream& operator << (ostream &os, const SqMat<N, floating> &m)
{
	int j;

	for (j=0;j<N;j++)
	{
		os<<m[j];
	}
	return os;
}

template <int N, class floating> 
void upperright2lowerleft(SqMat<N,floating> &M)
{
	for (int i=1; i<N; i++)
	{
		for (int j=0; j<i; j++)
		{
			M[i][j] = M[j][i];
		}
	}
}

template <int N, class floating> 
void lowerleft2upperright(SqMat<N,floating> &M)
{
	for (int i=1; i<N; i++)
	{
		for (int j=0; j<i; j++)
		{
			M[j][i] = M[i][j];
		}
	}
}

#endif
