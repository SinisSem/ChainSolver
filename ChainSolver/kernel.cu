#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <cusp/csr_matrix.h>
#include <cusp/coo_matrix.h>
#include <cusp/io/matrix_market.h>
#include <cusp/krylov/gmres.h>
#include <cusp/monitor.h>
#include "../MatrixGenerator/MatrixGen.h"

//__global__ void crpcrKernel(float *d_a,
//	float *d_b,
//	float *d_c,
//	float *d_d,
//	float *d_x,
//	unsigned int systemSizeOriginal,
//	unsigned int iterations)
//{
//	const unsigned int thid = threadIdx.x;
//	const unsigned int blid = blockIdx.x;
//	const unsigned int systemSize = blockDim.x * 2;
//	const unsigned int restSystemSize = blockDim.x;
//
//	extern __shared__ char shared[];
//
//	T* a = (T*)shared;
//	T* b = (T*)&a[systemSize + 1];
//	T* c = (T*)&b[systemSize + 1];
//	T* d = (T*)&c[systemSize + 1];
//	T* x = (T*)&d[systemSize + 1];
//
//	a[thid] = d_a[thid + blid * systemSizeOriginal];
//	b[thid] = d_b[thid + blid * systemSizeOriginal];
//	c[thid] = d_c[thid + blid * systemSizeOriginal];
//	d[thid] = d_d[thid + blid * systemSizeOriginal];
//
//	if (thid < (systemSizeOriginal - systemSize / 2))
//	{
//		d[thid + blockDim.x] = d_d[thid + blockDim.x + blid * systemSizeOriginal];
//		b[thid + blockDim.x] = d_b[thid + blockDim.x + blid * systemSizeOriginal];
//		c[thid + blockDim.x] = d_c[thid + blockDim.x + blid * systemSizeOriginal];
//		a[thid + blockDim.x] = d_a[thid + blockDim.x + blid * systemSizeOriginal];
//	}
//	else
//	{
//		d[thid + blockDim.x] = 0;
//		b[thid + blockDim.x] = 1;
//		c[thid + blockDim.x] = 0;
//		a[thid + blockDim.x] = 1;
//	}
//	__syncthreads();
//
//	int i = 2 * thid + 1;
//	if (i == systemSize - 1)
//	{
//		T tmp = a[i] / b[i - 1];
//		b[i] = b[i] - c[i - 1] * tmp;
//		d[i] = d[i] - d[i - 1] * tmp;
//		a[i] = -a[i - 1] * tmp;
//		c[i] = 0;
//	}
//	else
//	{
//		T tmp1 = a[i] / b[i - 1];
//		T tmp2 = c[i] / b[i + 1];
//		b[i] = b[i] - c[i - 1] * tmp1 - a[i + 1] * tmp2;
//		d[i] = d[i] - d[i - 1] * tmp1 - d[i + 1] * tmp2;
//		a[i] = -a[i - 1] * tmp1;
//		c[i] = -c[i + 1] * tmp2;
//	}
//
//	__syncthreads();
//
//	T* aa = (T*)&x[systemSize + 1];
//	T* bb = (T*)&aa[restSystemSize];
//	T* cc = (T*)&bb[restSystemSize];
//	T* dd = (T*)&cc[restSystemSize];
//	T* xx = (T*)&dd[restSystemSize];
//
//
//	aa[thid] = a[thid * 2 + 1];
//	bb[thid] = b[thid * 2 + 1];
//	cc[thid] = c[thid * 2 + 1];
//	dd[thid] = d[thid * 2 + 1];
//
//	__syncthreads();
//
//	T aNew, bNew, cNew, dNew;
//	int delta = 1;
//
//	//parallel cyclic reduction
//	for (unsigned int j = 0; j < iterations; j++)
//	{
//		int i = thid;
//		if (i < delta)
//		{
//			T tmp2 = cc[i] / bb[i + delta];
//			bNew = bb[i] - aa[i + delta] * tmp2;
//			dNew = dd[i] - dd[i + delta] * tmp2;
//			aNew = 0;
//			cNew = -cc[i + delta] * tmp2;
//		}
//		else if ((restSystemSize - i - 1) < delta)
//		{
//			T tmp = aa[i] / bb[i - delta];
//			bNew = bb[i] - cc[i - delta] * tmp;
//			dNew = dd[i] - dd[i - delta] * tmp;
//			aNew = -aa[i - delta] * tmp;
//			cNew = 0;
//		}
//		else
//		{
//			T tmp1 = aa[i] / bb[i - delta];
//			T tmp2 = cc[i] / bb[i + delta];
//			bNew = bb[i] - cc[i - delta] * tmp1 - aa[i + delta] * tmp2;
//			dNew = dd[i] - dd[i - delta] * tmp1 - dd[i + delta] * tmp2;
//			aNew = -aa[i - delta] * tmp1;
//			cNew = -cc[i + delta] * tmp2;
//		}
//		__syncthreads();
//
//		bb[i] = bNew;
//		dd[i] = dNew;
//		aa[i] = aNew;
//		cc[i] = cNew;
//
//		delta *= 2;
//
//		__syncthreads();
//	}
//
//	if (thid < delta)
//	{
//		int addr1 = thid;
//		int addr2 = thid + delta;
//		T tmp3 = bb[addr2] * bb[addr1] - cc[addr1] * aa[addr2];
//		xx[addr1] = (bb[addr2] * dd[addr1] - cc[addr1] * dd[addr2]) / tmp3;
//		xx[addr2] = (dd[addr2] * bb[addr1] - dd[addr1] * aa[addr2]) / tmp3;
//	}
//
//	__syncthreads();
//
//	x[thid * 2 + 1] = xx[thid];
//
//	__syncthreads();
//
//	//backward substitution    
//	i = 2 * thid;
//	if (i == 0)
//		x[i] = (d[i] - c[i] * x[i + 1]) / b[i];
//	else
//		x[i] = (d[i] - a[i] * x[i - 1] - c[i] * x[i + 1]) / b[i];
//
//	__syncthreads();
//
//	d_x[thid + blid * systemSizeOriginal] = x[thid];
//
//	if (thid < (systemSizeOriginal - systemSize / 2))
//		d_x[thid + blockDim.x + blid * systemSizeOriginal] = x[thid + blockDim.x];
//}



int main()
{
	cusp::coo_matrix<int, float, cusp::host_memory> A1;
	cusp::io::read_matrix_market_file(A1, "..\\Tests\\testMcut_2014.10.20_1,47,7.txt");
	cusp::csr_matrix<int, float, cusp::host_memory> A = A1;
	cusp::array1d<float, cusp::host_memory> B;
	cusp::io::read_matrix_market_file(B, "..\\Tests\\testVcut_2014.10.20_1,47,7.txt");

	

	return 0;
}