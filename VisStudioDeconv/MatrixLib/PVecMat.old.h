#ifndef PVecMat_h
#define PVecMat_h

#include <algorithm>
#include <iostream>

using std::max;
using std::min;
using std::ostream;
using std::endl;

#include <memory.h>

template <class floating> 
floating *allocate_vector(size_t n)
{
	return new floating[n];
}

template <class floating> 
void free_vector(floating *&vec)
{
	delete[] vec;
	vec = (floating*)NULL;
}

template <class floating>
floating **allocate_matrix(size_t m, size_t n)
{
	floating **mat = new floating *[m];
	for (size_t i = 0; i<m; i++)
	{
		mat[i] = allocate_vector<floating>(n);
	}
	return mat;
}

template <class floating>
void free_matrix(floating **& mat, size_t m)
{
	for (size_t i = 0; i<m; i++)
	{
		free_vector(mat[i]);
	}
	delete[] mat;
	mat = (floating **)NULL;
}

template <class floating> 
// lhs[] = rhs[]
void assign(floating *lhs, const floating *rhs, size_t n)
{
	memcpy(lhs, rhs, n*sizeof(floating));
}

template <class floating> 
void assign(floating **lhs, const floating *const*rhs, size_t m , size_t n)
// lhs[][] = rhs[][]
{
	for (size_t i = 0; i<m; i++)
	{
		assign(lhs[i], rhs[i], n);
	}
}

template <class floating> 
void randomize(floating *vec, size_t n)
{
	for (size_t i=0; i<n; i++)
	{
		vec[i] = rand()/(floating)RAND_MAX;
	}
}

template <class floating> 
void randomize(floating **mat, size_t m, size_t n)
{
	for (size_t i=0; i<m; i++)
	{
		randomize(mat[i], n);
	}
}

template <class floating> 
floating sum(const floating *vec, size_t n)
{
	floating s = (floating)0;
	for (size_t i=0; i<n; i++)
	{
		s += vec[i];
	}
	return s;
}

template <class floating> 
floating sum_except(const floating *vec, size_t n, size_t ex)
{
	if (ex < 0 || ex >= n) return sum(vec, n);
	else return sum(vec, ex) + sum(vec+ex+1, n-1-ex);
}

/*
template <class floating>
floating max(const floating *vec, size_t n)
{
	floating res = vec[0];
	for (size_t i = 1; i<n; i++)
	{
		res = max(res, vec[i]);
	}
	return res;
}

template <class floating>
floating max_except(const floating *vec, size_t n, size_t ex)
{
	if (ex < 0 || ex >= n) return max(vec, n);
	else if (ex == 0) return max(vec+1, n-1);
	else if (ex == n-1) return max(vec, n-1);
	else return max(max(vec, ex), max(vec+ex+1, n-1-ex));
}

template <class floating>
floating min(const floating *vec, size_t n)
{
	floating res = vec[0];
	for (size_t i = 1; i<n; i++)
	{
		res = min(res, vec[i]);
	}
	return res;
}

template <class floating>
floating min_except(const floating *vec, size_t n, size_t ex)
{
	if (ex < 0 || ex >= n) return min(vec, n);
	else if (ex == 0) return min(vec+1, n-1);
	else if (ex == n-1) return min(vec, n-1);
	else return min(min(vec, ex), min(vec+ex+1, n-1-ex));
}
*/

template <class floating>
floating width(const floating *vec, size_t n)
{
	return 2 * max(fabs(max(vec, n)), fabs(min(vec, n)));
}

template <class floating> 
void add(floating *lhs, const floating *rhs1, const floating *rhs2, size_t n)
// lhs[] = rhs1[] + rhs2[]
{
	floating *pl = lhs;
	const floating *pr1 = rhs1;
	const floating *pr2 = rhs2;

	for (size_t i = 0; i < n; i++)
	{
		*pl = (*pr1) + (*pr2);
		pl ++;
		pr1 ++;
		pr2 ++;
	}
}

template <class floating> 
void subtract(floating *lhs, const floating *rhs1, const floating *rhs2, size_t n)
// lhs[] = rhs1[] - rhs2[]
{
	floating *pl = lhs;
	const floating *pr1 = rhs1;
	const floating *pr2 = rhs2;

	for (size_t i = 0; i < n; i++)
	{
		*pl = (*pr1) - (*pr2);
		pl ++;
		pr1 ++;
		pr2 ++;
	}
}

template <class floating> 
void increment(floating *lhs, const floating *rhs, size_t n)
// lhs[] += rhs[]
{
	floating *pl = lhs;
	const floating *pr = rhs;
	
	for (size_t i = 0; i < n; i++)
	{
		*pl += *pr;
		pl ++;
		pr ++;
	}
}

template <class floating> 
void decrement(floating *lhs, const floating *rhs, size_t n)
// lhs[] -= rhs[]
{
	floating *pl = lhs;
	const floating *pr = rhs;
	
	for (size_t i = 0; i < n; i++)
	{
		*pl -= *pr;
		pl ++;
		pr ++;
	}
}

template <class floating> 
void multiply(floating *lhs, const floating *rhs, floating scaler, size_t n)
// lhs[] = scaler * rhs[]
{
	floating *pl = lhs;
	const floating *pr = rhs;
	
	for (size_t i = 0; i < n; i++)
	{
		*pl = (*pr) * scaler;
		pl ++;
		pr ++;
	}
}

template <class floating> 
void multiply_assign(floating *lhs, floating scaler, size_t n)
// lhs[] *= scaler
{
	floating *pl = lhs;
	
	for (size_t i = 0; i < n; i++)
	{
		*pl *= scaler;
		pl ++;
	}
}

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

#endif
