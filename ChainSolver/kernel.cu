
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <cusp/csr_matrix.h>
#include <cusp/coo_matrix.h>
#include <cusp/io/matrix_market.h>
#include <cusp/krylov/gmres.h>
#include <cusp/monitor.h>

int main()
{
	cusp::coo_matrix<int, float, cusp::host_memory> A;
	cusp::io::read_matrix_market_file(A, "..\\Tests\\testM_2014.10.20_1,47,7.txt");
	cusp::csr_matrix<int, float, cusp::host_memory> A1 = A;
	cusp::array1d<float, cusp::host_memory> B;
	cusp::io::read_matrix_market_file(B, "..\\Tests\\testV_2014.10.20_1,47,7.txt");
	cusp::array1d<float, cusp::host_memory> X = B;
	cusp::verbose_monitor<float> monitor(B, 100, 1e-6);
	cusp::krylov::gmres(A1, X, B, 50, monitor);
	return 0;
}
// --сошлось за 81 итерацию!!!