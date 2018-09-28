//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sqrt.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 25-Sep-2018 16:02:46
//

// Include Files
#include "rt_nonfinite.h"
#include "func_FC.h"
#include "sqrt.h"

// Function Definitions

//
// Arguments    : emxArray_real_T *x
// Return Type  : void
//
void b_sqrt(emxArray_real_T *x)
{
  int i5;
  int k;
  i5 = x->size[1];
  for (k = 0; k < i5; k++) {
    x->data[k] = sqrt(x->data[k]);
  }
}

//
// File trailer for sqrt.cpp
//
// [EOF]
//
