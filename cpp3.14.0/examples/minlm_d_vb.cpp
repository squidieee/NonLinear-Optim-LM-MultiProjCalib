#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "optimization.h"

/*
* minlm_d_vb example, which shows how to do optimization with boundary constraints
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
}

int main(int argc, char **argv)
{
	//
	// This example demonstrates minimization of F(x0,x1) = f0^2+f1^2, where 
	//
	//     f0(x0,x1) = 10*(x0+3)^2
	//     f1(x0,x1) = (x1-3)^2
	//
	// with boundary constraints
	//
	//     -1 <= x0 <= +1
	//     -1 <= x1 <= +1
	//
	// using "V" mode of the Levenberg-Marquardt optimizer.
	//
	// Optimization algorithm uses:
	// * function vector f[] = {f1,f2}
	//
	// No other information (Jacobian, gradient, etc.) is needed.
	//
	real_1d_array x = "[0,0]";
	real_1d_array bndl = "[-1,-1]";
	real_1d_array bndu = "[+1,+1]";
	double epsx = 0.0000000001;
	ae_int_t maxits = 0;
	minlmstate state;
	minlmreport rep;

	minlmcreatev(2, x, 0.0001, state);
	minlmsetbc(state, bndl, bndu);
	minlmsetcond(state, epsx, maxits);
	alglib::minlmoptimize(state, function1_fvec);
	minlmresults(state, x, rep);

	printf("%s\n", x.tostring(2).c_str()); // EXPECTED: [-1,+1]
	return 0;
}