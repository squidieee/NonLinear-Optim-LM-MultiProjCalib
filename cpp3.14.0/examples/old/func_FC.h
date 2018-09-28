//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: func_FC.h
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 25-Sep-2018 16:02:46
//
#ifndef __FUNC_FC_H__
#define __FUNC_FC_H__

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "func_FC_types.h"

// Function Declarations
extern void func_FC(const double p[23], const emxArray_real_T *x_proj, const
                    emxArray_real_T *x_cam, emxArray_real_T *err,
                    emxArray_real_T *A, emxArray_real_T *B, emxArray_real_T *C);

#endif

//
// File trailer for func_FC.h
//
// [EOF]
//
