//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: rodrigues.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 25-Sep-2018 16:02:46
//

// Include Files
#include "rt_nonfinite.h"
#include "func_FC.h"
#include "rodrigues.h"
#include "norm.h"

// Function Definitions

//
// RODRIGUES Transform rotation matrix into rotation vector and viceversa.
//
//   Sintax:  [OUT]=RODRIGUES(IN)
//    If IN is a 3x3 rotation matrix then OUT is the
//   corresponding 3x1 rotation vector
//    if IN is a rotation 3-vector then OUT is the
//   corresponding 3x3 rotation matrix
// Arguments    : const double in[3]
//                double out[9]
//                double dout[27]
// Return Type  : void
//
void rodrigues(const double in[3], double out[9], double dout[27])
{
  double theta;
  int k;
  static const double dv4[27] = { 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0,
    0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 0.0,
    0.0, 0.0, 0.0 };

  signed char I[9];
  double omega[3];
  double c;
  double alpha;
  double beta;
  double b_gamma;
  double omegav[9];
  double A[9];
  int i0;
  double dm1dm2[84];
  static const signed char a[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  double dRdm1[189];
  static const signed char b[81] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

  double b_dRdm1[36];
  double b_a[16];
  int i1;
  static const signed char iv1[4] = { 0, 0, 0, 1 };

  double c_dRdm1[36];
  double b_I[12];

  // %
  // %		Copyright (c) March 1993 -- Pietro Perona
  // %		California Institute of Technology
  // %
  // % ALL CHECKED BY JEAN-YVES BOUGUET, October 1995.
  // % FOR ALL JACOBIAN MATRICES !!! LOOK AT THE TEST AT THE END !!
  // % BUG when norm(om)=pi fixed -- April 6th, 1997;
  // % Jean-Yves Bouguet
  // % Add projection of the 3x3 matrix onto the set of special ortogonal matrices SO(3) by SVD -- February 7th, 2003; 
  // % Jean-Yves Bouguet
  //  BUG FOR THE CASE norm(om)=pi fixed by Mike Burl on Feb 27, 2007
  // bigeps = 10e+4*eps;
  // % it is a rotation vector
  theta = norm(in);
  if (theta < 2.2204460492503131E-16) {
    memset(&out[0], 0, 9U * sizeof(double));
    for (k = 0; k < 3; k++) {
      out[k + 3 * k] = 1.0;
    }

    // if nargout > 1,
    memcpy(&dout[0], &dv4[0], 27U * sizeof(double));

    // end;
  } else {
    // % make it a column vec. if necess.
    // m3 = [in,theta]
    for (k = 0; k < 9; k++) {
      I[k] = 0;
    }

    for (k = 0; k < 3; k++) {
      I[k + 3 * k] = 1;
      omega[k] = in[k] / theta;
    }

    // m2 = [omega;theta]
    c = theta * theta;
    alpha = cos(theta);
    beta = sin(theta);
    b_gamma = 1.0 - cos(theta);
    omegav[0] = 0.0;
    omegav[3] = -omega[2];
    omegav[6] = omega[1];
    omegav[1] = omega[2];
    omegav[4] = 0.0;
    omegav[7] = -omega[0];
    omegav[2] = -omega[1];
    omegav[5] = omega[0];
    omegav[8] = 0.0;
    for (k = 0; k < 3; k++) {
      for (i0 = 0; i0 < 3; i0++) {
        A[k + 3 * i0] = omega[k] * omega[i0];
      }
    }

    // m1 = [alpha;beta;gamma;omegav;A];
    memset(&dm1dm2[0], 0, 84U * sizeof(double));
    dm1dm2[63] = -sin(theta);
    dm1dm2[64] = cos(theta);
    dm1dm2[65] = sin(theta);
    for (k = 0; k < 3; k++) {
      memcpy(&dm1dm2[3 + 21 * k], &dv4[9 * k], 9U * sizeof(double));
    }

    dm1dm2[12] = 2.0 * omega[0];
    dm1dm2[13] = omega[1];
    dm1dm2[14] = omega[2];
    dm1dm2[15] = omega[1];
    dm1dm2[16] = 0.0;
    dm1dm2[17] = 0.0;
    dm1dm2[18] = omega[2];
    dm1dm2[19] = 0.0;
    dm1dm2[20] = 0.0;
    dm1dm2[33] = 0.0;
    dm1dm2[34] = omega[0];
    dm1dm2[35] = 0.0;
    dm1dm2[36] = omega[0];
    dm1dm2[37] = 2.0 * omega[1];
    dm1dm2[38] = omega[2];
    dm1dm2[39] = 0.0;
    dm1dm2[40] = omega[2];
    dm1dm2[41] = 0.0;
    dm1dm2[54] = 0.0;
    dm1dm2[55] = 0.0;
    dm1dm2[56] = omega[0];
    dm1dm2[57] = 0.0;
    dm1dm2[58] = 0.0;
    dm1dm2[59] = omega[1];
    dm1dm2[60] = omega[0];
    dm1dm2[61] = omega[1];
    dm1dm2[62] = 2.0 * omega[2];
    for (k = 0; k < 9; k++) {
      out[k] = ((double)a[k] * alpha + omegav[k] * beta) + A[k] * b_gamma;
    }

    memset(&dRdm1[0], 0, 189U * sizeof(double));
    for (k = 0; k < 3; k++) {
      dRdm1[k << 2] = 1.0;
    }

    memcpy(&dRdm1[9], &omegav[0], 9U * sizeof(double));
    for (k = 0; k < 9; k++) {
      for (i0 = 0; i0 < 9; i0++) {
        dRdm1[i0 + 9 * (3 + k)] = beta * (double)b[i0 + 9 * k];
      }
    }

    memcpy(&dRdm1[18], &A[0], 9U * sizeof(double));
    for (k = 0; k < 9; k++) {
      for (i0 = 0; i0 < 9; i0++) {
        dRdm1[i0 + 9 * (12 + k)] = b_gamma * (double)b[i0 + 9 * k];
      }
    }

    for (k = 0; k < 9; k++) {
      for (i0 = 0; i0 < 4; i0++) {
        b_dRdm1[k + 9 * i0] = 0.0;
        for (i1 = 0; i1 < 21; i1++) {
          b_dRdm1[k + 9 * i0] += dRdm1[k + 9 * i1] * dm1dm2[i1 + 21 * i0];
        }
      }
    }

    for (k = 0; k < 3; k++) {
      for (i0 = 0; i0 < 3; i0++) {
        b_a[i0 + (k << 2)] = (double)a[i0 + 3 * k] / theta;
      }
    }

    for (k = 0; k < 3; k++) {
      b_a[12 + k] = -in[k] / c;
    }

    for (k = 0; k < 4; k++) {
      b_a[3 + (k << 2)] = iv1[k];
    }

    for (k = 0; k < 9; k++) {
      for (i0 = 0; i0 < 4; i0++) {
        c_dRdm1[k + 9 * i0] = 0.0;
        for (i1 = 0; i1 < 4; i1++) {
          c_dRdm1[k + 9 * i0] += b_dRdm1[k + 9 * i1] * b_a[i1 + (i0 << 2)];
        }
      }
    }

    for (k = 0; k < 3; k++) {
      for (i0 = 0; i0 < 3; i0++) {
        b_I[i0 + (k << 2)] = I[i0 + 3 * k];
      }

      b_I[3 + (k << 2)] = in[k] / theta;
    }

    for (k = 0; k < 9; k++) {
      for (i0 = 0; i0 < 3; i0++) {
        dout[k + 9 * i0] = 0.0;
        for (i1 = 0; i1 < 4; i1++) {
          dout[k + 9 * i0] += c_dRdm1[k + 9 * i1] * b_I[i1 + (i0 << 2)];
        }
      }
    }
  }

  //     %% it is prob. a rot matr.
  // % test of the Jacobians:
  // % TEST OF dRdom:
  // % TEST OF dOmdR:
  // % OTHER BUG: (FIXED NOW!!!)
  // % NORMAL OPERATION
  // % Test: norm(om) = pi
  // % Another test case where norm(om)=pi from Chen Feng (June 27th, 2014)
  // % Another test case where norm(om)=pi from 余成义 (July 1st, 2014)
}

//
// File trailer for rodrigues.cpp
//
// [EOF]
//
