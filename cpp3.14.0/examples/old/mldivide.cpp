//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: mldivide.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 25-Sep-2018 16:02:46
//

// Include Files
#include "rt_nonfinite.h"
#include "func_FC.h"
#include "mldivide.h"
#include "func_FC_emxutil.h"

// Function Declarations
static void eml_lusolve(const double A[9], const emxArray_real_T *B,
  emxArray_real_T *X);

// Function Definitions

//
// Arguments    : const double A[9]
//                const emxArray_real_T *B
//                emxArray_real_T *X
// Return Type  : void
//
static void eml_lusolve(const double A[9], const emxArray_real_T *B,
  emxArray_real_T *X)
{
  double b_A[9];
  signed char ipiv[3];
  int k;
  int j;
  int c;
  int iy;
  int ix;
  double temp;
  double s;
  int jBcol;
  int kAcol;
  int nb;
  memcpy(&b_A[0], &A[0], 9U * sizeof(double));
  for (k = 0; k < 3; k++) {
    ipiv[k] = (signed char)(1 + k);
  }

  for (j = 0; j < 2; j++) {
    c = j << 2;
    iy = 0;
    ix = c;
    temp = fabs(b_A[c]);
    for (k = 1; k + 1 <= 3 - j; k++) {
      ix++;
      s = fabs(b_A[ix]);
      if (s > temp) {
        iy = k;
        temp = s;
      }
    }

    if (b_A[c + iy] != 0.0) {
      if (iy != 0) {
        ipiv[j] = (signed char)((j + iy) + 1);
        ix = j;
        iy += j;
        for (k = 0; k < 3; k++) {
          temp = b_A[ix];
          b_A[ix] = b_A[iy];
          b_A[iy] = temp;
          ix += 3;
          iy += 3;
        }
      }

      k = (c - j) + 3;
      for (iy = c + 1; iy + 1 <= k; iy++) {
        b_A[iy] /= b_A[c];
      }
    }

    iy = c;
    jBcol = c + 3;
    for (kAcol = 1; kAcol <= 2 - j; kAcol++) {
      temp = b_A[jBcol];
      if (b_A[jBcol] != 0.0) {
        ix = c + 1;
        k = (iy - j) + 6;
        for (nb = 4 + iy; nb + 1 <= k; nb++) {
          b_A[nb] += b_A[ix] * -temp;
          ix++;
        }
      }

      jBcol += 3;
      iy += 3;
    }
  }

  k = X->size[0] * X->size[1];
  X->size[0] = 3;
  X->size[1] = B->size[1];
  emxEnsureCapacity((emxArray__common *)X, k, (int)sizeof(double));
  iy = B->size[0] * B->size[1];
  for (k = 0; k < iy; k++) {
    X->data[k] = B->data[k];
  }

  nb = B->size[1];
  for (iy = 0; iy < 2; iy++) {
    if (ipiv[iy] != iy + 1) {
      jBcol = ipiv[iy] - 1;
      for (kAcol = 0; kAcol + 1 <= nb; kAcol++) {
        temp = X->data[iy + X->size[0] * kAcol];
        X->data[iy + X->size[0] * kAcol] = X->data[jBcol + X->size[0] * kAcol];
        X->data[jBcol + X->size[0] * kAcol] = temp;
      }
    }
  }

  if ((B->size[1] == 0) || (X->size[1] == 0)) {
  } else {
    for (j = 1; j <= nb; j++) {
      jBcol = 3 * (j - 1);
      for (k = 0; k < 3; k++) {
        kAcol = 3 * k;
        if (X->data[k + jBcol] != 0.0) {
          for (iy = k + 1; iy + 1 < 4; iy++) {
            X->data[iy + jBcol] -= X->data[k + jBcol] * b_A[iy + kAcol];
          }
        }
      }
    }
  }

  if ((B->size[1] == 0) || (X->size[1] == 0)) {
  } else {
    for (j = 1; j <= nb; j++) {
      jBcol = 3 * (j - 1);
      for (k = 2; k > -1; k += -1) {
        kAcol = 3 * k;
        if (X->data[k + jBcol] != 0.0) {
          temp = X->data[k + jBcol];
          X->data[k + jBcol] = temp / b_A[k + kAcol];
          for (iy = 0; iy + 1 <= k; iy++) {
            X->data[iy + jBcol] -= X->data[k + jBcol] * b_A[iy + kAcol];
          }
        }
      }
    }
  }
}

//
// Arguments    : const double A[9]
//                const emxArray_real_T *B
//                emxArray_real_T *Y
// Return Type  : void
//
void mldivide(const double A[9], const emxArray_real_T *B, emxArray_real_T *Y)
{
  int i2;
  if (B->size[1] == 0) {
    i2 = Y->size[0] * Y->size[1];
    Y->size[0] = 3;
    Y->size[1] = 0;
    emxEnsureCapacity((emxArray__common *)Y, i2, (int)sizeof(double));
  } else {
    eml_lusolve(A, B, Y);
  }
}

//
// File trailer for mldivide.cpp
//
// [EOF]
//
