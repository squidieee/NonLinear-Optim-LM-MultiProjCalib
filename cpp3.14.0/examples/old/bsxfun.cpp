//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: bsxfun.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 25-Sep-2018 16:02:46
//

// Include Files
#include "rt_nonfinite.h"
#include "func_FC.h"
#include "bsxfun.h"
#include "func_FC_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *a
//                const double b[3]
//                emxArray_real_T *c
// Return Type  : void
//
void b_bsxfun(const emxArray_real_T *a, const double b[3], emxArray_real_T *c)
{
  int unnamed_idx_1;
  int asub;
  int ak;
  int ck;
  double av[3];
  int k;
  double cv[3];
  unnamed_idx_1 = c->size[0] * c->size[1];
  c->size[0] = 3;
  c->size[1] = a->size[1];
  emxEnsureCapacity((emxArray__common *)c, unnamed_idx_1, (int)sizeof(double));
  unnamed_idx_1 = a->size[1];
  if (unnamed_idx_1 == 0) {
  } else {
    asub = 1;
    ak = 0;
    unnamed_idx_1 = a->size[1];
    unnamed_idx_1 = 3 * unnamed_idx_1 - 3;
    for (ck = 0; ck <= unnamed_idx_1; ck += 3) {
      for (k = 0; k < 3; k++) {
        av[k] = a->data[ak + k];
      }

      for (k = 0; k < 3; k++) {
        cv[k] = av[k] + b[k];
      }

      for (k = 0; k < 3; k++) {
        c->data[ck + k] = cv[k];
      }

      if (asub < a->size[1]) {
        ak += 3;
        asub++;
      } else {
        asub = 1;
      }
    }
  }
}

//
// Arguments    : const emxArray_real_T *a
//                const emxArray_real_T *b
//                emxArray_real_T *c
// Return Type  : void
//
void bsxfun(const emxArray_real_T *a, const emxArray_real_T *b, emxArray_real_T *
            c)
{
  int k;
  int i4;
  int asub;
  int bsub;
  int ak;
  int bk;
  int ck;
  double av[3];
  double b_b;
  double cv[3];
  if (a->size[1] <= b->size[1]) {
    k = a->size[1];
  } else {
    k = b->size[1];
  }

  i4 = c->size[0] * c->size[1];
  c->size[0] = 3;
  c->size[1] = k;
  emxEnsureCapacity((emxArray__common *)c, i4, (int)sizeof(double));
  if (k == 0) {
  } else {
    asub = 1;
    bsub = 1;
    ak = 0;
    bk = 0;
    i4 = 3 * k - 3;
    for (ck = 0; ck <= i4; ck += 3) {
      for (k = 0; k < 3; k++) {
        av[k] = a->data[ak + k];
      }

      b_b = b->data[bk];
      for (k = 0; k < 3; k++) {
        cv[k] = av[k] * b_b;
      }

      for (k = 0; k < 3; k++) {
        c->data[ck + k] = cv[k];
      }

      if (asub < a->size[1]) {
        ak += 3;
        bk++;
        bsub++;
        asub++;
      } else if (bsub < b->size[1]) {
        bk++;
        bsub++;
      } else {
        asub = 1;
        bsub = 1;
      }
    }
  }
}

//
// File trailer for bsxfun.cpp
//
// [EOF]
//
