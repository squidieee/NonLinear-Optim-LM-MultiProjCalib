//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: rdivide.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 25-Sep-2018 16:02:46
//

// Include Files
#include "rt_nonfinite.h"
#include "func_FC.h"
#include "rdivide.h"
#include "func_FC_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *x
//                const emxArray_real_T *y
//                emxArray_real_T *z
// Return Type  : void
//
void b_rdivide(const emxArray_real_T *x, const emxArray_real_T *y,
               emxArray_real_T *z)
{
  int i3;
  int loop_ub;
  i3 = z->size[0] * z->size[1];
  z->size[0] = 1;
  z->size[1] = x->size[1];
  emxEnsureCapacity((emxArray__common *)z, i3, (int)sizeof(double));
  loop_ub = x->size[0] * x->size[1];
  for (i3 = 0; i3 < loop_ub; i3++) {
    z->data[i3] = x->data[i3] / y->data[i3];
  }
}

//
// Arguments    : double x
//                double y
// Return Type  : double
//
double rdivide(double x, double y)
{
  return x / y;
}

//
// File trailer for rdivide.cpp
//
// [EOF]
//
