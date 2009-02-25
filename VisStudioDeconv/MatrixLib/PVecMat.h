#ifndef PVecMat_h
#define PVecMat_h

#include <algorithm>
#include <iostream>
#include <fstream>

using std::max;
using std::min;
using std::ofstream;
using std::endl;

#include <memory.h>
#include <math.h>

//column-major indexing for matrix
inline int sub2ind(int r, int c, int H) { return c*H+r; }
inline void ind2sub(int i, int &r, int &c, int H) { r = i%H; c = i/H; }

template <class floating> 
// lhs[] = rhs[]
inline void assign(floating *lhs, floating a, size_t n)
{
	const floating *lhs_last = lhs + n;
	for (; lhs != lhs_last; ++lhs) *lhs = a;
}

template <class floating> 
inline void assign(floating *lhs, floating a, size_t m , size_t n)
// lhs[][] = rhs[][]
{
	const floating *lhs_last = lhs + m*n;
	for (; lhs != lhs_last; ++lhs) *lhs = a;
}

template <class floating> 
// lhs[] = rhs[]
inline void assign(floating *lhs, const floating *rhs, size_t n)
{
	memcpy(lhs, rhs, n*sizeof(floating));
}

template <class floating> 
inline void assign(floating *lhs, const floating *rhs, size_t m , size_t n)
// lhs[][] = rhs[][]
{
	memcpy(lhs, rhs, (m*n)*sizeof(floating));
}

template <class floating> 
void randomize(floating *vec, size_t n)
{
	const floating *vec_last = vec + n;
	for (; vec != vec_last; ++vec) *vec = (floating)rand()/(floating)RAND_MAX;
}

template <class floating> 
void randomize(floating *mat, size_t m, size_t n)
{
	randomize(mat, m*n);
}

template <class floating> 
floating sum(const floating *vec, size_t n)
{
	floating s = (floating)0;
	const floating *vec_last = vec + n;	
	for (; vec != vec_last; ++vec) s += *vec;

	return s;
}

template <class floating> 
floating prod(const floating *vec, size_t n)
{
	floating s = (floating)1;
	const floating *vec_last = vec + n;	
	for (; vec != vec_last; ++vec) s *= *vec;

	return s;
}

template <class floating>
size_t min_elem_ind(const floating *vec, size_t n)
{
	floating minval = vec[0];
	size_t minind = 0;
	for (size_t i = 1; i < n; ++i)
	{
		if (vec[i] < minval)
		{
			minval = vec[i];
			minind = i;
		}
	}
	return minind;
}

template <class floating>
size_t max_elem_ind(const floating *vec, size_t n)
{
	floating maxval = vec[0];
    size_t maxind = 0;
	for (size_t i = 1; i < n; ++i)
	{
		if (vec[i] > maxval)
		{
			maxval = vec[i];
			maxind = i;
		}
	}
	return maxind;
}

/*
template <class floating> 
floating sum_except(const floating *vec, size_t n, size_t ex)
{
	if (ex < 0 || ex >= n) return sum(vec, n);
	else return sum(vec, ex) + sum(vec+ex+1, n-1-ex);
}
*/

template <class floating>
floating max_elem(const floating *vec, size_t n)
{
	floating res = *vec;
	const floating *vec_last = vec+n;
	for (++vec; vec!=vec_last; ++vec) 
		res = max(res, *vec);
	return res;
}

/*
template <class floating>
floating max_except(const floating *vec, size_t n, size_t ex)
{
	if (ex < 0 || ex >= n) return max(vec, n);
	else if (ex == 0) return max(vec+1, n-1);
	else if (ex == n-1) return max(vec, n-1);
	else return max(max(vec, ex), max(vec+ex+1, n-1-ex));
}
*/


template <class floating>
floating min_elem(const floating *vec, size_t n)
{
	floating res = *vec;
	const floating *vec_last = vec+n;
	for (++vec; vec!=vec_last; ++vec) 
		res = min(res, *vec);
	return res;
}

/*
template <class floating>
floating min_except(const floating *vec, size_t n, size_t ex)
{
	if (ex < 0 || ex >= n) return min(vec, n);
	else if (ex == 0) return min(vec+1, n-1);
	else if (ex == n-1) return min(vec, n-1);
	else return min(min(vec, ex), min(vec+ex+1, n-1-ex));
}
*/

/*
template <class floating>
floating width(const floating *vec, size_t n)
{
	return 2 * max(fabs(max(vec, n)), fabs(min(vec, n)));
}
*/

template <class floating>
void max_elemwise(floating *lhs, const floating *rhs, size_t n)
// lhs[] = max(lhs[], rhs[]);
{
	const floating *lhs_last = lhs+n;
	for (; lhs!=lhs_last; ++lhs, ++rhs) 
		*lhs = max(*lhs, *rhs);	
}

template <class floating>
void min_elemwise(floating *lhs, const floating *rhs, size_t n)
// lhs[] = min(lhs[], rhs[]);
{
	const floating *lhs_last = lhs+n;
	for (; lhs!=lhs_last; ++lhs, ++rhs) 
		*lhs = min(*lhs, *rhs);	
}

template <class floating> 
void add(floating *lhs, const floating *rhs1, const floating *rhs2, size_t n)
// lhs[] = rhs1[] + rhs2[]
{
	const floating *lhs_last = lhs + n;
	for (; lhs != lhs_last; ++lhs, ++rhs1, ++rhs2) *lhs = (*rhs1) + (*rhs2);
}

template <class floating> 
void subtract(floating *lhs, const floating *rhs1, const floating *rhs2, size_t n)
// lhs[] = rhs1[] - rhs2[]
{
	const floating *lhs_last = lhs + n;
	for (; lhs != lhs_last; ++lhs, ++rhs1, ++rhs2) *lhs = (*rhs1) - (*rhs2);
}

template <class floating> 
void increment(floating *lhs, const floating *rhs, size_t n)
// lhs[] += rhs[]
{
	const floating *lhs_last = lhs + n;
	for (; lhs != lhs_last; ++lhs, ++rhs) *lhs += *rhs;
}

template <class floating> 
void increment(floating *lhs, const floating &rhs, size_t n)
// lhs[] += rhs
{
	const floating *lhs_last = lhs + n;
	for (; lhs != lhs_last; ++lhs) *lhs += rhs;
}

template <class floating> 
void decrement(floating *lhs, const floating *rhs, size_t n)
// lhs[] -= rhs[]
{
	const floating *lhs_last = lhs + n;
	for (; lhs != lhs_last; ++lhs, ++rhs) *lhs -= *rhs;
}

template <class floating> 
void multiply(floating *lhs, const floating *rhs, floating scaler, size_t n)
// lhs[] = scaler * rhs[]
{
	const floating *lhs_last = lhs + n;
	for (; lhs != lhs_last; ++lhs, ++rhs) *lhs = (*rhs) * scaler;
}

template <class floating> 
void multiply_assign(floating *lhs, floating scaler, size_t n)
// lhs[] *= scaler
{
	const floating *lhs_last = lhs + n;
	for (; lhs != lhs_last; ++lhs) *lhs *= scaler;
}

template <class floating> 
void multiply_elem_assign(floating *lhs, const floating *rhs, size_t n)
// lhs[] *= rhs[]
{
	const floating *lhs_last = lhs + n;
	for (; lhs != lhs_last; ++lhs, ++rhs) *lhs *= (*rhs);
}

template <class floating> 
void move(floating *pnt, const double *start, const floating *direction, floating scaler, size_t n)
// pnt[] += start[] + scaler * direction[]
{
	const floating *pnt_last = pnt + n;
	for (; pnt != pnt_last; ++pnt, ++start, ++direction) *pnt = (*start) + (*direction) * scaler;
}

template <class floating> 
void move_assign(floating *pnt, const floating *direction, floating scaler, size_t n)
// pnt[] += scaler * direction[]
{
	const floating *pnt_last = pnt + n;
	for (; pnt != pnt_last; ++pnt, ++direction) *pnt += (*direction) * scaler;
}

template <class floating>
floating inner(const floating *a, const floating *b, size_t n)
{
	floating sum = (floating)0;
	const floating *a_last = a + n;
	for (; a != a_last; ++a, ++b) sum += (*a) * (*b); 

	return sum;
}

template <class floating>
floating inner(const floating *a, size_t n)
{
	floating sum = (floating)0;
	const floating *a_last = a + n;
	for (; a != a_last; ++a) sum += (*a) * (*a); 

	return sum;
}

template <class floating> 
void multiply(floating *lhs, const floating *rhs1, size_t m, size_t n, const floating *rhs2)
// lhs[] = rhs1[][] * rhs2[]
{
	memset(lhs, 0, m*sizeof(floating));
	const floating *rhs2_last = rhs2 + n;
	for (; rhs2 != rhs2_last; ++rhs2, rhs1+=m) move_assign(lhs, rhs1, *rhs2, m);
}

template <class floating> 
void multiply(floating *lhs, const floating *rhs1, const floating *rhs2, size_t m, size_t n)
// lhs[] = rhs1[] * rhs2[][]
{
	const floating *lhs_last = lhs + n;
	for (; lhs != lhs_last; ++lhs, rhs2+=m) *lhs = inner(rhs1,rhs2,m);
}

template <class floating>
void solve(floating *sol, floating *A, floating *b, size_t n)
{
	assign(sol, b, n);

	for (size_t i = 1; i < n; i++)
	{
		floating vp = A[(i-1)*n+(i-1)];
		for (size_t j = i; j < n; j++)
		{
			move_assign(A+j*n+i, A+(i-1)*n+i, -A[j*n+(i-1)]/vp, n-i);
		}
		move_assign(sol+i, A+(i-1)*n+i, -sol[i-1]/vp, n-i);
		memset(A+(i-1)*n+i, 0, (n-i)*sizeof(floating));
	}

	for (size_t i = n-1; i > 0; i--)
	{
		sol[i] /= A[i*n+i];
		move_assign(sol, A+i*n, -sol[i], i);
	}
	sol[0] /= A[0];
}

template <class floating>
void solve(floating *sol, const floating *A, const floating *b, size_t n)
{
	//RefCntMem<floating> A0(n*n);
	//RefCntMem<floating> b0(n);

	floating *A0 = new floating[n*n];
	floating *b0 = new floating[n];

	assign((floating*)A0, (floating*)A, n, n);
	assign((floating*)b0, (floating*)b, n);

	solve(sol,(floating*)A0, (floating*)b0, n);

	delete[] A0;
	delete[] b0;
}

template <class floating> 
void multiply(floating *lhs, const floating *rhs1, const floating *rhs2, size_t l, size_t m, size_t n)
// lhs[][] = rhs1[][] * rhs2[][]
{
	const floating *lhs_last = lhs + l*n;
	for (; lhs != lhs_last; lhs += l, rhs2 += m) multiply(lhs, rhs1, rhs2, l, m);
}

template <class floating> 
void transpose(floating *lhs, const floating *rhs, size_t m, size_t n)
{
	for (size_t i = 0; i<m; i++)
	{
		for (size_t j = 0; j<n; j++)
		{
			lhs[i*n+j] = rhs[j*m+i];
		}
	}
}

template <class floating> 
void transpose_assign(floating *mat, size_t n)
{
	for (size_t i = 1; i<n; i++)
	{
		for (size_t j = 0; j<i; j++)
		{			
			swap(mat[j*n+i], mat[i*n+j]);
		}
	}
}

template <class floating>
void upperright2lowerleft(floating *mat, size_t n)
{
	for (size_t i = 1; i < n; i++) //row index
	{
		for (size_t j = 0; j < i; j++) //column index
		{
			mat[j*n+i] = mat[i*n+j];
		}
	}
}

template <class floating> 
void lowerleft2upperright(floating *mat, size_t n)
{
	for (size_t i = 1; i < n; i++) //row index
	{
		for (size_t j = 0; j < i; j++) //column index
		{
			mat[i*n+j] = mat[j*n+i];
		}
	}
}

template <class floating>
floating norm_inf(const floating *vec, size_t n)
{
	floating res = abs(*vec);
	const floating *vecLast = vec+n;
	for (++vec; vec!=vecLast; ++vec) 
		res = max(abs(*vec), res);
	return res;
}

template <class floating>
floating norm_L2(const floating *vec, size_t n)
{
	floating res = (*vec)*(*vec);
	const floating *vecLast = vec+n;
	for (++vec; vec!=vecLast; ++vec) 
		res += (*vec)*(*vec);
	return (floating)sqrt(res);
}

template <class T, class S>
S vec_ssd(const T *a, const T *b, size_t n)
{
	S ret = 0;
	const T *a_last = a + n;
	for (; a!=a_last; ++a, ++b) 
	{
		S dif = *a;
		dif -= *b;
		ret += dif*dif;
	}
	return ret;
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
		count += (abs(*vec) > t);
		vec ++;
	}
	return count;
}

template <class floating> 
void output_vector(const char *name, const floating *vec, size_t m, 
				   const char *ele_delim = "\t")
{
	size_t L = strlen(name);
	const char *ext = name+L-3;

	if (strcmp(ext, "txt")==0 || strcmp(ext, "TXT")==0)
	{
		ofstream os(name);
		for (size_t i = 0; i<m; i++)
		{
			os << vec[i] << ele_delim;				
		}
		os.close();
	}
	else if (strcmp(ext, "dat")==0 || strcmp(ext, "DAT")==0)
	{
		FILE *fp = 0;
		fp = fopen(name, "wb");
		if (fp==0)
		{ 
			printf("failed to open %s\n", name);
			return;
		} 
		fwrite(vec, sizeof(floating), m, fp);
		fclose(fp);
	}
	else 
	{
		printf("unsupported output file format: %s\n", name);
	}
}

template <class floating> 
void output_matrix(const char *name, const floating *mat, size_t m, size_t n, bool col_major = true, 
				   const char *row_delim = "\n", const char *col_delim = "\t")
				   //mat is column major if col_major == 1 otherwise row major
				   //m: # rows 
				   //n: # columns
				   //output is column major for dat file
				   //output is row major for txt file
{
	size_t L = strlen(name);
	const char *ext = name+L-3;

	if (strcmp(ext, "txt")==0 || strcmp(ext, "TXT")==0)
	{
		ofstream os(name);
		if (col_major)
		{
			for (size_t i = 0; i<m; i++)
			{
				for (size_t j = 0; j<n; j++)
				{
					os << mat[j*m+i] << col_delim;
				}
				os << row_delim;
			}
		}
		else 
		{
			for (size_t i = 0; i<m; i++)
			{
				for (size_t j = 0; j<n; j++)
				{
					os << mat[i*n+j] << col_delim;
				}
				os << row_delim;
			}
		}
		os.close();
	}
	else if (strcmp(ext, "dat")==0 || strcmp(ext, "DAT")==0)
	{
		FILE *fp = 0;
		fp = fopen(name, "wb");
		if (fp==0)
		{ 
			printf("failed to open %s\n", name);
			return;
		} 

		if (col_major)
		{
			fwrite(mat, sizeof(floating), n*m, fp);
		}
		else
		{
			floating *matt = new floating[m*n];
			transpose(matt, mat, m, n);
			fwrite(matt, sizeof(floating), n*m, fp);
			delete[] matt;
		}
		fclose(fp);
	}
	else 
	{
		printf("unsupported output file format: %s\n", name);
	}
}
#endif
