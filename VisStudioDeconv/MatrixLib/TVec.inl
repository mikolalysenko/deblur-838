#ifndef TVec_inl
#define TVec_inl

template <int N, class floating> 
Vec <N,floating> Vec<N,floating>::zero(void)
{
	Vec <N,floating> z;
	int d;
	for (d=0;d<N;d++)
	{
		z[d] = (floating)0;
	}
	return z;
}

template <int N, class floating> 
Vec <N,floating> Vec<N,floating>::unit(int i)
{
	Vec <N,floating> u = Vec<N,floating>::zero();
	u[i] = (floating)1;
	return u;
}

template <int N, class floating> 
Vec <N,floating> ::Vec( const floating vec[N])
{
	int d;
	for (d=0;d<N;d++)
	{
		v[d] = vec[d];
	}
}

template <int N, class floating> 
Vec <N,floating> ::Vec( const Vec <N,floating> &vec)
{
	int d;
	for (d=0;d<N;d++)
	{
		v[d] = vec[d];
	}
}

template <int N, class floating>
bool Vec <N,floating> ::isconstant(floating f) const
{
	int d;
	for (d=0;d<N;d++)
	{
		if (v[d] != f)
		{
			return false;
		}
	}
	return true;
}

template <int N, class floating>
bool Vec <N,floating> ::iszero(void) const
{
	int d;
	for (d=0;d<N;d++)
	{
		if (v[d] != (floating)0)
		{
			return false;
		}
	}
	return true;
}

template <int N, class floating>
bool Vec<N,floating>::operator == (const Vec <N, floating> &vec) const
{
	int d;
	for (d=0;d<N;d++)
	{
		if (v[d] != vec[d])
		{
			return false;
		}
	}
	return true;
}

template <int N, class floating>
void Vec<N,floating>::makezero(void)
{
	int d;
	for (d=0;d<N;d++)
	{
		v[d] = (floating)0;
	}
}

template <int N, class floating>
void Vec<N,floating>::makeunit(int i)
{
	makezero();
	v[i] = (floating)1;
}

template <int N, class floating> 
Vec <N, floating> &Vec<N,floating>::operator = (const Vec <N, floating> &vec)
{
	int d;
	for (d=0;d<N;d++)
	{
		v[d] = vec[d];
	}
	return *this;
}

template <int N, class floating> 
Vec <N, floating> &Vec<N,floating>::operator = (const floating &f)
{
	int d;
	for (d=0;d<N;d++)
	{
		v[d] = f;
	}
	return *this;
}

template <int N, class floating> 
Vec <N, floating>& Vec<N,floating>::operator += (const Vec <N, floating> &vec)
{
	int d;
	for (d=0;d<N;d++)
	{
		v[d] += vec[d];
	}
	return *this;
}

template <int N, class floating> 
Vec <N, floating> &Vec<N,floating>::operator -= (const Vec <N, floating> &vec)
{
	int d;
	for (d=0;d<N;d++)
	{
		v[d] -= vec[d];
	}
	return *this;
}

template <int N, class floating> 
Vec <N, floating> &Vec<N,floating>::operator *= (floating s)
{
	int d;
	for (d=0;d<N;d++)
	{
		v[d] *= s;
	}
	return *this;
}

template <int N, class floating>
Vec <N, floating> operator - (const Vec <N, floating>&v)
{
	Vec <N, floating> n;
	int d;
	for (d=0;d<N;d++)
	{
		n[d]=-v[d];
	}
	return n;
}

template <int N, class floating>
floating inner(const Vec <N,floating> &a, const Vec <N,floating> &b)
{
	floating sum=0;
	int d;
	for (d=0;d<N;d++)
	{
		sum+=a[d]*b[d];
	}
	return sum;
}

template <class floating> 
Vec<3, floating> cross(const Vec<3,floating> &a, const Vec<3,floating> &b)
{
	Vec<3, floating> v;
	v[0] = a[1] * b[2] - a[2] * b[1];
	v[1] = a[2] * b[0] - a[0] * b[2];
	v[2] = a[0] * b[1] - a[1] * b[0];
	return v;
}

template <int N, class floating>
istream& operator >> (istream &is, Vec<N, floating> &m)
{
	int j;

	for (j=0;j<N;j++)
	{
		is>>m[j];
	}
	return is;
}

template <int N, class floating>
ostream& operator << (ostream &os, const Vec<N, floating> &m)
{
	int j;

	for (j=0;j<N;j++)
	{
		os<<m[j]<<"\t";
	}
	//os<<endl;
	return os;
}

#endif
