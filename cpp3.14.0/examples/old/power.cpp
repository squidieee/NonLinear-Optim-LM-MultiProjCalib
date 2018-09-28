//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: power.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 25-Sep-2018 16:02:46
//

// Include Files
#include "rt_nonfinite.h"
#include "func_FC.h"
#include "power.h"
#include "func_FC_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *a
//                emxArray_real_T *y
// Return Type  : void
//
void power(const emxArray_real_T *a, emxArray_real_T *y)
{
  unsigned int uv0[2];
  int k;
  for (k = 0; k < 2; k++) {
    uv0[k] = (unsigned int)a->size[k];
  }

  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = (int)uv0[1];
  emxEnsureCapacity((emxArray__common *)y, k, (int)sizeof(double));
  for (k = 0; k < (int)uv0[1]; k++) {
    y->data[k] = a->data[k] * a->data[k];
  }
}

//
// File trailer for power.cpp
//
// [EOF]
//
