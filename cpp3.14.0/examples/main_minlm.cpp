#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "optimization.h"
#include <opencv2/opencv.hpp>

/* 
* minlm_d_vj example, which shows how to do optimization using analytic Jacobian
* http://www.alglib.net/optimization/levenbergmarquardt.php#downloadsection
*/

using namespace alglib;

const int NPROJ = 4; // replace with global setting
std::vector<std::vector<cv::Point2f>> m_cam_pts; // replace with displaycalibration member Make sure it is double
std::vector<std::vector<cv::Point2f>> m_proj_pts; // replace with displaycalibration member

void function_mincost(const std::vector<float>& params, std::vector<cv::Point2f>& cam_pts, std::vector<cv::Point2f>& proj_pts, std::vector<double>& err, cv::Mat_<double> & jac_A, cv::Mat_<double>& jac_B, cv::Mat_<double>& jac_C);

// TEST FUNCTION
bool loadBlobData(const std::string& file_name, const int proj_idx)
{
	using namespace cv;
	Mat cam_blob, proj_blob;
	FileStorage fs(file_name, FileStorage::READ);

	if (!fs.isOpened())
	{
		throw std::runtime_error("loadBlobData() Failed to open " + file_name);
	}

	fs["cam_pts"] >> cam_blob;
	fs["proj_pts"] >> proj_blob;

	cam_blob.reshape(2).copyTo(m_cam_pts[proj_idx]);	
	proj_blob.reshape(2).copyTo(m_proj_pts[proj_idx]);

	fs.release();
	return !m_cam_pts[proj_idx].empty();
}

void func_err(const real_1d_array &params, real_1d_array &err, void *ptr)
{
	std::vector<double> all_err;
	all_err.clear();
	cv::Mat *p = static_cast<cv::Mat*>(ptr);
	p->release();
	
	for (int pi = 0; pi < NPROJ; pi++)
	{
		/// prepare params
		std::vector<float> param_pi;
		// get projector pi's 10 params
		for (int vi = pi * 10; vi < pi * 10 + 10; vi++)
		{
			param_pi.push_back(params[vi]);
		}
		// get camera and sphere 13 params 
		for (int vi = NPROJ * 10; vi < NPROJ * 10 + 13; vi++)
		{
			param_pi.push_back(params[vi]);
		}
		/// compute the error and jacobian matrix
		std::vector<double> err_pi;
		cv::Mat_<double> A, B, C;
		function_mincost(param_pi, m_cam_pts[pi], m_proj_pts[pi], err_pi, A, B, C);
		/// attach to err array
		all_err.insert(all_err.end(), err_pi.begin(), err_pi.end());
		/// compute jacobian
		cv::Mat_<double> J_A_zero;
		J_A_zero = cv::Mat::zeros(A.rows, NPROJ * 10, CV_64FC1);
		J_A_zero(cv::Range(0, J_A_zero.rows), cv::Range(pi * 10, pi * 10 + 10)) = A;
		cv::Mat_<double> J_pi;
		cv::hconcat(J_A_zero, B, J_pi);
		cv::hconcat(J_pi, C, J_pi);
		if (p->empty())
		{
			*p = J_pi;
		}
		else
			cv::vconcat(*p, J_pi, *p);
	}
	/// transfer to local data type real_1d_array
	err.setcontent(all_err.size(), all_err.data());	
}

void func_jac(const real_1d_array &x, real_1d_array &fi, real_2d_array &jac, void *ptr)
{
	// todo: transfer opencv jac matrix to real_2d_array
}


void  function1_fvec(const real_1d_array &x, real_1d_array &fi, void *ptr)
{
	////
	//// this callback calculates
	//// f0(x0,x1) = 100*(x0+3)^4,
	//// f1(x0,x1) = (x1-3)^4
	////
	//fi[0] = 10 * pow(x[0] + 3, 2);
	//fi[1] = pow(x[1] - 3, 2);
	//int *p = static_cast<int*>(ptr);
	//std::cout << p[0] << std::endl;
}
void  function1_jac(const real_1d_array &x, real_1d_array &fi, real_2d_array &jac, void *ptr)
{
	////
	//// this callback calculates
	//// f0(x0,x1) = 100*(x0+3)^4,
	//// f1(x0,x1) = (x1-3)^4
	//// and Jacobian matrix J = [dfi/dxj]
	////
	//fi[0] = 10 * pow(x[0] + 3, 2);
	//fi[1] = pow(x[1] - 3, 2);
	//jac[0][0] = 20 * (x[0] + 3);
	//jac[0][1] = 0;
	//jac[1][0] = 0;
	//jac[1][1] = 2 * (x[1] - 3);
	//int *p = static_cast<int*>(ptr);
	//std::cout << p[1] << std::endl;
}

int main(int argc, char **argv)
{
	// step 1: read xml files to m_cam_pts and m_proj_pts
	m_cam_pts.clear();
	m_cam_pts.resize(NPROJ);
	m_proj_pts.clear();
	m_proj_pts.resize(NPROJ);
	
	for(int i=0;i<NPROJ;i++)
	{
		std::string file_name = "Proj" + std::to_string(i+1) +"PairBlobData.xml";
		if (!loadBlobData(file_name, i))
		{
			std::cout << "zero vector found" << std::endl;
			return -1;
		}
		
	}
	// todo : convert to emxarray

	// step 2: hard-code initial guess vector
	double p0[] = { 996.51215, 1002.67977, 512.06846, 771.93374, 1.10698, -0.04101, 0.01969, 0.07523, 0.27022, 0.95986, 996.51215, 1002.67977, 512.06846, 771.93374, 0.16425, 1.67746, -2.59486, -0.11809, 0.30305, 1.04268, 996.51215, 1002.67977, 512.06846, 771.93374, -0.96035, -0.25020, 2.72969, 0.58512, -0.29760, 0.52548, 996.51215, 1002.67977, 512.06846, 771.93374, 0.16586, -0.71275, -0.11698, 0.73967, -0.36260, 0.65142, -0.02915, -0.00076, 1.56098, 1.11870, 1026.27057, 1028.92285, 664.43693, 498.78765, -0.37914, 0.25287, -0.15159, 0.00042, -0.00094 };
	//float p0[] = { 996.51215, 1002.67977, 512.06846, 771.93374, 1.10698, -0.04101, 0.01969, 0.07523, 0.27022, 0.95986, -0.02915, -0.00076, 1.56098, 1.11870, 1026.27057, 1028.92285, 664.43693, 498.78765, -0.37914, 0.25287, -0.15159, 0.00042, -0.00094 };
	///TEST: test if func_FC works: yes it works
	//std::vector<float> params (p0, p0 + sizeof(p0)/sizeof(float));
	//std::vector<float> err;

	//cv::Mat_<double> A, B, C;
	//function_mincost(params, m_cam_pts[0], m_proj_pts[0], err, A, B, C);

	// step 3: optimize
	//real_1d_array param;
	//param.setcontent(53, p0);

	//double epsx = 0.0000000001;
	//ae_int_t maxits = 0;
	//minlmstate state;
	//minlmreport rep;

	//minlmcreatevj(2, param, state);
	real_1d_array err, params;
	cv::Mat Jac;
	params.setcontent(53, p0);
	func_err(params, err, &Jac);
	
	////
	//// This example demonstrates minimization of F(x0,x1) = f0^2+f1^2, where 
	////
	////     f0(x0,x1) = 10*(x0+3)^2
	////     f1(x0,x1) = (x1-3)^2
	////
	//// using "VJ" mode of the Levenberg-Marquardt optimizer.
	////
	//// Optimization algorithm uses:
	//// * function vector f[] = {f1,f2}
	//// * Jacobian matrix J = {dfi/dxj}.
	////
	//real_1d_array x = "[0,0]";  // initial vector
	//double epsx = 0.0000000001; // The subroutine finishes its work if on k+1-th iteration the condition |v| <= EpsX is fulfilled
	//							//Recommended values: 1E-9 ... 1E-12
	//ae_int_t maxits = 0;
	//minlmstate state;
	//minlmreport rep;

	//int a[2] = { 0,1 };
	//minlmcreatevj(2, x, state); // create vector + jacobian optimization scheme
	//minlmsetcond(state, epsx, maxits); // set stop condition: If MaxIts=0, the number of iterations is unlimited.
	//alglib::minlmoptimize(state, function1_fvec, function1_jac, NULL, &a); // Running algorithm
	//minlmresults(state, x, rep); // obtain results

	//printf("%s\n", x.tostring(2).c_str()); // EXPECTED: [-3,+3]
	return 0;
}