#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "optimization.h"

/* 
* minlm_d_vj example, which shows how to do optimization using analytic Jacobian
* http://www.alglib.net/optimization/levenbergmarquardt.php#downloadsection
*/

using namespace alglib;
void  function1_fvec(const real_1d_array &x, real_1d_array &fi, void *ptr)
{
	//
	// this callback calculates
	// f0(x0,x1) = 100*(x0+3)^4,
	// f1(x0,x1) = (x1-3)^4
	//
	fi[0] = 10 * pow(x[0] + 3, 2);
	fi[1] = pow(x[1] - 3, 2);
	int *p = static_cast<int*>(ptr);
	std::cout << p[0] << std::endl;
}
void  function1_jac(const real_1d_array &x, real_1d_array &fi, real_2d_array &jac, void *ptr)
{
	//
	// this callback calculates
	// f0(x0,x1) = 100*(x0+3)^4,
	// f1(x0,x1) = (x1-3)^4
	// and Jacobian matrix J = [dfi/dxj]
	//
	fi[0] = 10 * pow(x[0] + 3, 2);
	fi[1] = pow(x[1] - 3, 2);
	jac[0][0] = 20 * (x[0] + 3);
	jac[0][1] = 0;
	jac[1][0] = 0;
	jac[1][1] = 2 * (x[1] - 3);
	int *p = static_cast<int*>(ptr);
	std::cout << p[1] << std::endl;
}


int main(int argc, char **argv)
{
	//
	// This example demonstrates minimization of F(x0,x1) = f0^2+f1^2, where 
	//
	//     f0(x0,x1) = 10*(x0+3)^2
	//     f1(x0,x1) = (x1-3)^2
	//
	// using "VJ" mode of the Levenberg-Marquardt optimizer.
	//
	// Optimization algorithm uses:
	// * function vector f[] = {f1,f2}
	// * Jacobian matrix J = {dfi/dxj}.
	//
	real_1d_array x = "[0,0]";  // initial vector
	double epsx = 0.0000000001; // The subroutine finishes its work if on k+1-th iteration the condition |v| <= EpsX is fulfilled
								//Recommended values: 1E-9 ... 1E-12
	ae_int_t maxits = 0;
	minlmstate state;
	minlmreport rep;

	int a[2] = { 0,1 };
	minlmcreatevj(2, x, state); // create vector + jacobian optimization scheme
	minlmsetcond(state, epsx, maxits); // set stop condition: If MaxIts=0, the number of iterations is unlimited.
	alglib::minlmoptimize(state, function1_fvec, function1_jac, NULL, &a); // Running algorithm
	minlmresults(state, x, rep); // obtain results

	printf("%s\n", x.tostring(2).c_str()); // EXPECTED: [-3,+3]
	return 0;
}