#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "optimization.h"
#include <opencv2/opencv.hpp>

using namespace alglib;

const int NPROJ = 4; // replace with global setting
std::vector<std::vector<cv::Point2f>> m_cam_pts; // replace with displaycalibration member Make sure it is double
std::vector<std::vector<cv::Point2f>> m_proj_pts; // replace with displaycalibration member

void function_mincost(const std::vector<float>& params, std::vector<cv::Point2f>& cam_pts, std::vector<cv::Point2f>& proj_pts, std::vector<double>& err, bool needJac, cv::Mat_<double>& jac_A = cv::Mat_<double>(), cv::Mat_<double>& jac_B = cv::Mat_<double>(), cv::Mat_<double>& jac_C = cv::Mat_<double>());

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

void func_err(const real_1d_array &params, real_1d_array &fi, void *ptr)
{
	std::vector<double> all_err;
	all_err.clear();

	for (int pi = 0; pi < NPROJ; pi++)
	{
		/// step 1: prepare params
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
		/// step 2: compute the error
		std::vector<double> err_pi;
		function_mincost(param_pi, m_cam_pts[pi], m_proj_pts[pi], err_pi, false);
		/// step 3: attach to err array
		all_err.insert(all_err.end(), err_pi.begin(), err_pi.end());
	}
	/// step 4: transfer to local data type real_1d_array
	for (int i = 0; i < all_err.size(); i++)
	{
		fi[i] = all_err[i];
	}
	double fx_err = cv::norm(all_err);
	std::cout << "errV "<< fx_err << std::endl;
}

/// This function is called by minlmoptimize before func_err
void func_jac(const real_1d_array &params, real_1d_array &fi, real_2d_array &jac, void *ptr)
{
	std::vector<double> all_err;
	all_err.clear();
	cv::Mat_<double> Jac;
	for (int pi = 0; pi < NPROJ; pi++)
	{
		/// step 1: prepare params
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
		/// step 2: compute the error and sub-jacobian matrix
		std::vector<double> err_pi;
		cv::Mat_<double> A, B, C;
		function_mincost(param_pi, m_cam_pts[pi], m_proj_pts[pi], err_pi, true, A, B, C);
		/// step 3: attach to err array
		all_err.insert(all_err.end(), err_pi.begin(), err_pi.end());
		/// step 4: attach to overall jacobian
		cv::Mat_<double> J_A_zero;
		J_A_zero = cv::Mat::zeros(A.rows, NPROJ * 10, CV_64FC1);
		cv::Mat_<double> subJ_A = J_A_zero(cv::Range(0, J_A_zero.rows), cv::Range(pi * 10, pi * 10 + 10));
		A.copyTo(subJ_A);

		cv::Mat_<double> J_pi;
		cv::hconcat(J_A_zero, B, J_pi);
		cv::hconcat(J_pi, C, J_pi);
		if (Jac.empty())
		{
			Jac = J_pi.clone();
		}
		else
			cv::vconcat(Jac, J_pi, Jac);
	}
	/// step 5: transfer to local data type real_1d_array: fi
	for (int i = 0; i < all_err.size(); i++)
	{
		fi[i] = all_err[i];
	}
	/// step 6: transfer to local data type real_1d_array: Jac
	for (int i = 0; i < Jac.rows; i++)
	{
		for (int j = 0; j < Jac.cols; j++)
		{
			jac[i][j] = Jac.at<double>(i, j);
		}
	}
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
	//double p0[] = { 1016.87820,1029.38485,507.92715,754.08783,1.10656,-0.05432,0.01538,0.11139,0.36497,0.84614,1023.35620,1035.13364,499.92493,744.55160,0.17864,1.65197,-2.62566,-0.06159,0.39637,0.92306,1020.33377,1005.98899,489.76017,788.66973,-0.94338,-0.23343,2.73796,0.61566,-0.32681,0.43906,1018.48668,1004.88681,474.77748,774.45156,0.17243,-0.69404,-0.11015,0.79895,-0.33946,0.53568,-0.02809,0.00076,1.51831,1.04394,1052.61151,1047.93345,666.54580,490.87032,-0.30752,0.12802,-0.01481,0.00103,0.00094 };
	///TEST: test if func_FC works: yes it works
	//std::vector<float> params (p0, p0 + sizeof(p0)/sizeof(float));
	//std::vector<float> err;

	//cv::Mat_<double> A, B, C;
	//function_mincost(params, m_cam_pts[0], m_proj_pts[0], err, A, B, C);

	// step 3: optimize
	std::vector<double> pvec(p0, p0 + sizeof(p0) / sizeof(double));
	real_1d_array params;
	params.setcontent(53, pvec.data());

	double epsx = 0.000000001;
	ae_int_t maxits = 0;
	minlmstate state;
	minlmreport rep;
	real_1d_array bndl, bndu;
	double RATIO = 5;
	bndl.setlength(params.length());
	bndu.setlength(params.length());
	for (int i = 0; i < params.length(); i++)
	{
		bndl[i] = params[i] - std::abs(params[i]) * RATIO;
		bndu[i] = params[i] + std::abs(params[i]) * RATIO;
	}

	unsigned int NUM_BLOB(0);

	for(int i = 0;i < NPROJ; i++)
	{
		NUM_BLOB += m_cam_pts[i].size();
	}
	try {
		alglib::minlmcreatevj(NUM_BLOB * 2, params, state);
		//alglib::minlmcreatev(NUM_BLOB*2, params, 0.0001, state);
		//minlmsetbc(state, bndl, bndu);
		minlmsetacctype(state, 1);
		alglib::minlmsetcond(state, epsx, maxits); // set stop condition: If MaxIts=0, the number of iterations is unlimited.
		//alglib::minlmoptimize(state, func_err); // Running algorithm
		minlmoptimize(state, func_err, func_jac, NULL); // Running algorithm
		alglib::minlmresults(state, params, rep); // obtain results
	}
	catch (alglib::ap_error& e) { std::cerr << e.msg << std::endl; }
	printf("%s\n", params.tostring(6).c_str()); // EXPECTED: [-3,+3]
	return 0;
}