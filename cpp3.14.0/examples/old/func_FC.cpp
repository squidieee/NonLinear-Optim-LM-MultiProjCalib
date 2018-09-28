//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: func_FC.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 25-Sep-2018 16:02:46
//

// Include Files
#include "rt_nonfinite.h"
#include "func_FC.h"
#include "JdXdS.h"
#include "JdXdT.h"
#include "JdXdR.h"
#include "JdXdcp.h"
#include "JdXdfp.h"
#include "JdxdX.h"
#include "func_FC_emxutil.h"
#include "power.h"
#include "rdivide.h"
#include "bsxfun.h"
#include "sqrt.h"
#include "norm.h"
#include "mldivide.h"
#include "rodrigues.h"

/* Cost function called for each projector with blob pair data from camera and this projector
 * It computes error emxArray_real_T *err from blob pair data
 * It also computes jacobian matrix A B C regarding projector, sphere pose and camera params respectively
*/

// parameter definition:
// p = [ fx_p, fy_p, cx_p, cy_p, //1-4
//        om1, om2, om3, //5-7
//        t1, t2, t3, //8-10
//        a, b, c, r, //11-14
//        fx_c, fy_c, cx_c, cy_c, k1_c, k2_c, k3_c, p1_c, p2_c] //15-23
//  p has to be a column vector
//  Return:
//        A : NumberOfBlobs x 10 ->projector intrinsic and extrinsic
//        B : NumberOfBlobs x 4 ->sphere pose
//        C : NumberOfBlobs x 9 ->camera intrinsics (with lens distortions)
//        X : 3 x NumberOfBlobs ->3d pts
// Arguments    : const double p[23]
//                const emxArray_real_T *x_proj
//                const emxArray_real_T *x_cam
//                emxArray_real_T *err
//                emxArray_real_T *A
//                emxArray_real_T *B
//                emxArray_real_T *C
// Return Type  : void
//
void func_FC(const double p[23], const emxArray_real_T *x_proj, const
             emxArray_real_T *x_cam, emxArray_real_T *err, emxArray_real_T *A,
             emxArray_real_T *B, emxArray_real_T *C)
{
  double J_dRdom[27];
  double R[9];
  double r;
  int br;
  int n;
  double b_R[9];
  int ib;
  double b_p[9];
  double C_S[3];
  double b_C[3];
  static const signed char iv0[3] = { 0, 0, 1 };

  int cr;
  emxArray_real_T *b_x_proj;
  emxArray_real_T *V;
  emxArray_real_T *b_V;
  emxArray_real_T *a;
  emxArray_real_T *c_V;
  emxArray_real_T *r0;
  emxArray_real_T *d_V;
  emxArray_real_T *b2_4ac;
  double y[3];
  emxArray_real_T *b;
  unsigned int unnamed_idx_1;
  int b_n;
  int ic;
  int ar;
  int ia;
  double b_a;
  emxArray_real_T *r1;
  emxArray_real_T *lambda;
  emxArray_real_T *temp;
  emxArray_real_T *X;
  emxArray_boolean_T *x;
  emxArray_int32_T *ii;
  boolean_T exitg1;
  boolean_T guard1 = false;
  emxArray_real_T *c_a;
  emxArray_real_T *b_y;
  emxArray_real_T *c_y;
  emxArray_real_T *d_a;
  emxArray_real_T *e_V;
  emxArray_real_T *b_lambda;
  emxArray_real_T *b_X;
  emxArray_real_T *c_X;
  emxArray_real_T *x1;
  emxArray_real_T *d_X;
  emxArray_real_T *e_X;
  emxArray_real_T *b_y1;
  emxArray_real_T *r2;
  emxArray_real_T *c_p;
  double e_a;
  emxArray_real_T *b_err;
  double fx_p;
  double fy_p;
  double cx_p;
  double cy_p;
  double r11;
  double r12;
  double r13;
  double r21;
  double r22;
  double r23;
  double r31;
  double r32;
  double r33;
  double t1;
  double t2;
  double t3;
  double s1;
  double s2;
  double s3;
  double fx_c;
  double fy_c;
  double kc1;
  double kc2;
  double kc3;
  double pc1;
  double pc2;
  double J_dxdX[6];
  double b_t2;
  double b_t3;
  double t4;
  double t5;
  double t7;
  double t6;
  double t8;
  double t9;
  double t13;
  double b_x[4];
  double J_dxdfc[4];
  double c_x[10];
  double J_dxdkc[10];
  double dv0[6];
  double dv1[6];
  double dv2[27];
  double b_J_dxdX[18];
  double c_J_dxdX[4];
  double d_J_dxdX[4];
  double e_J_dxdX[6];
  double f_J_dxdX[6];
  double dv3[12];
  static const signed char J_dxdcc[4] = { 1, 0, 0, 1 };

  rodrigues(*(double (*)[3])&p[4], R, J_dRdom);
  r = p[13];

  // % prepare pixels from projector %%
  // global DATA_DIR;
  // file_path = [DATA_DIR '\Proj' num2str(proj_idx) 'PairBlobData.xml'];
  // xmlfile = xml2struct(file_path);
  // str = xmlfile.Children(8).Children(8).Children.Data;
  // data = str2num(regexprep(str,'\r\n|\n|\r',''));
  // pt2 = data(:) -1;
  // x_proj = reshape(pt2,2,length(pt2)/2); % 2 by number of pts
  br = x_proj->size[1];
  if (2 >= br) {
    br = 2;
  }

  if (0 == x_proj->size[1]) {
    n = 0;
  } else {
    n = br;
  }

  //  3 by N
  // % compute x_cam_est %%
  //  ray-sphere intersection
  for (ib = 0; ib < 3; ib++) {
    for (br = 0; br < 3; br++) {
      b_R[br + 3 * ib] = -R[ib + 3 * br];
    }
  }

  //  3 by 1
  b_p[0] = p[0];
  b_p[3] = 0.0;
  b_p[6] = p[2];
  b_p[1] = 0.0;
  b_p[4] = p[1];
  b_p[7] = p[3];
  for (ib = 0; ib < 3; ib++) {
    b_C[ib] = 0.0;
    for (br = 0; br < 3; br++) {
      b_C[ib] += b_R[ib + 3 * br] * p[7 + br];
    }

    //  3 by 1
    C_S[ib] = b_C[ib] - p[ib + 10];
    b_p[2 + 3 * ib] = iv0[ib];
  }

  for (ib = 0; ib < 3; ib++) {
    for (br = 0; br < 3; br++) {
      b_R[ib + 3 * br] = 0.0;
      for (cr = 0; cr < 3; cr++) {
        b_R[ib + 3 * br] += b_p[ib + 3 * cr] * R[cr + 3 * br];
      }
    }
  }

  emxInit_real_T(&b_x_proj, 2);
//  ib = b_x_proj->size[0] * b_x_proj->size[1];
  b_x_proj->size[0] = 3;
  b_x_proj->size[1] = x_proj->size[1];
  ib = b_x_proj->size[0] * b_x_proj->size[1];
  emxEnsureCapacity((emxArray__common *)b_x_proj, ib, (int)sizeof(double));
  cr = x_proj->size[1];
  for (ib = 0; ib < cr; ib++) {
    for (br = 0; br < 2; br++) {
      b_x_proj->data[br + b_x_proj->size[0] * ib] = x_proj->data[br +
        x_proj->size[0] * ib];
    }
  }

  for (ib = 0; ib < n; ib++) {
    b_x_proj->data[2 + b_x_proj->size[0] * ib] = 1.0;
  }

  emxInit_real_T(&V, 2);
  emxInit_real_T(&b_V, 2);
  mldivide(b_R, b_x_proj, V);

  //  vector: 3 by N
  cr = V->size[1];
//  ib = b_V->size[0] * b_V->size[1];
  b_V->size[0] = 1;
  b_V->size[1] = cr;
  ib = b_V->size[0] * b_V->size[1];
  emxEnsureCapacity((emxArray__common *)b_V, ib, (int)sizeof(double));
  emxFree_real_T(&b_x_proj);
  for (ib = 0; ib < cr; ib++) {
    b_V->data[b_V->size[0] * ib] = V->data[V->size[0] * ib];
  }

  emxInit_real_T(&a, 2);
  emxInit_real_T(&c_V, 2);
  power(b_V, a);
  cr = V->size[1];
//  ib = c_V->size[0] * c_V->size[1];
  c_V->size[0] = 1;
  c_V->size[1] = cr;
  ib = c_V->size[0] * c_V->size[1];
  emxEnsureCapacity((emxArray__common *)c_V, ib, (int)sizeof(double));
  emxFree_real_T(&b_V);
  for (ib = 0; ib < cr; ib++) {
    c_V->data[c_V->size[0] * ib] = V->data[1 + V->size[0] * ib];
  }

  emxInit_real_T(&r0, 2);
  emxInit_real_T(&d_V, 2);
  power(c_V, r0);
  cr = V->size[1];
//  ib = d_V->size[0] * d_V->size[1];
  d_V->size[0] = 1;
  d_V->size[1] = cr;
  ib = d_V->size[0] * d_V->size[1];
  emxEnsureCapacity((emxArray__common *)d_V, ib, (int)sizeof(double));
  emxFree_real_T(&c_V);
  for (ib = 0; ib < cr; ib++) {
    d_V->data[d_V->size[0] * ib] = V->data[2 + V->size[0] * ib];
  }

  emxInit_real_T(&b2_4ac, 2);
  power(d_V, b2_4ac);
//  ib = a->size[0] * a->size[1];
  a->size[0] = 1;
  ib = a->size[0] * a->size[1];
  emxEnsureCapacity((emxArray__common *)a, ib, (int)sizeof(double));
  br = a->size[0];
  cr = a->size[1];
  cr *= br;
  emxFree_real_T(&d_V);
  for (ib = 0; ib < cr; ib++) {
    a->data[ib] = (a->data[ib] + r0->data[ib]) + b2_4ac->data[ib];
  }

  //  row vector: 1 by N
  for (ib = 0; ib < 3; ib++) {
    y[ib] = 2.0 * C_S[ib];
  }

  emxInit_real_T(&b, 2);
  unnamed_idx_1 = (unsigned int)V->size[1];
  b_n = V->size[1] - 1;
  ib = b->size[0] * b->size[1];
  b->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)b, ib, (int)sizeof(double));
  ib = b->size[0] * b->size[1];
  b->size[1] = (int)unnamed_idx_1;
  emxEnsureCapacity((emxArray__common *)b, ib, (int)sizeof(double));
  cr = (int)unnamed_idx_1;
  for (ib = 0; ib < cr; ib++) {
    b->data[ib] = 0.0;
  }

  if (V->size[1] == 0) {
  } else {
    for (cr = 1; cr - 1 <= b_n; cr++) {
      for (ic = cr; ic <= cr; ic++) {
        b->data[ic - 1] = 0.0;
      }
    }

    br = 0;
    for (cr = 0; cr <= b_n; cr++) {
      ar = 0;
      for (ib = br; ib + 1 <= br + 3; ib++) {
        if (V->data[ib] != 0.0) {
          ia = ar;
          for (ic = cr; ic + 1 <= cr + 1; ic++) {
            ia++;
            b->data[ic] += V->data[ib] * y[ia - 1];
          }
        }

        ar++;
      }

      br += 3;
    }
  }

  //  row vector: 1 by N
  b_a = norm(C_S);

  //  row scalar: 1 by 1
  power(b, b2_4ac);
  b_a = 4.0 * (b_a * b_a - p[13] * p[13]);
  ib = b2_4ac->size[0] * b2_4ac->size[1];
  b2_4ac->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)b2_4ac, ib, (int)sizeof(double));
  br = b2_4ac->size[0];
  cr = b2_4ac->size[1];
  cr *= br;
  for (ib = 0; ib < cr; ib++) {
    b2_4ac->data[ib] -= b_a * a->data[ib];
  }

  //  vector: 1 by N
  ib = r0->size[0] * r0->size[1];
  r0->size[0] = 1;
  r0->size[1] = b2_4ac->size[1];
  emxEnsureCapacity((emxArray__common *)r0, ib, (int)sizeof(double));
  cr = b2_4ac->size[0] * b2_4ac->size[1];
  for (ib = 0; ib < cr; ib++) {
    r0->data[ib] = b2_4ac->data[ib];
  }

  emxInit_real_T(&r1, 2);
  b_sqrt(r0);
  ib = r1->size[0] * r1->size[1];
  r1->size[0] = 1;
  r1->size[1] = r0->size[1];
  emxEnsureCapacity((emxArray__common *)r1, ib, (int)sizeof(double));
  cr = r0->size[0] * r0->size[1];
  for (ib = 0; ib < cr; ib++) {
    r1->data[ib] = r0->data[ib] - b->data[ib];
  }

  emxInit_real_T(&lambda, 2);
  b_rdivide(r1, a, lambda);
  ib = lambda->size[0] * lambda->size[1];
  lambda->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)lambda, ib, (int)sizeof(double));
  br = lambda->size[0];
  cr = lambda->size[1];
  cr *= br;
  emxFree_real_T(&r1);
  for (ib = 0; ib < cr; ib++) {
    lambda->data[ib] /= 2.0;
  }

  emxInit_real_T(&temp, 2);
  emxInit_real_T(&X, 2);
  emxInit_boolean_T(&x, 2);

  //  vector: 1 by N
  bsxfun(V, lambda, temp);
  b_bsxfun(temp, b_C, X);

  //  C + V*lambda: vector 3 by N
  ib = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = b2_4ac->size[1];
  emxEnsureCapacity((emxArray__common *)x, ib, (int)sizeof(boolean_T));
  cr = b2_4ac->size[0] * b2_4ac->size[1];
  for (ib = 0; ib < cr; ib++) {
    x->data[ib] = (b2_4ac->data[ib] < 0.0);
  }

  emxInit_int32_T(&ii, 2);
  br = x->size[1];
  ar = 0;
  ib = ii->size[0] * ii->size[1];
  ii->size[0] = 1;
  ii->size[1] = x->size[1];
  emxEnsureCapacity((emxArray__common *)ii, ib, (int)sizeof(int));
  cr = 1;
  exitg1 = false;
  while ((!exitg1) && (cr <= br)) {
    guard1 = false;
    if (x->data[cr - 1]) {
      ar++;
      ii->data[ar - 1] = cr;
      if (ar >= br) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      cr++;
    }
  }

  if (x->size[1] == 1) {
    if (ar == 0) {
      ib = ii->size[0] * ii->size[1];
      ii->size[0] = 1;
      ii->size[1] = 0;
      emxEnsureCapacity((emxArray__common *)ii, ib, (int)sizeof(int));
    }
  } else {
    ib = ii->size[0] * ii->size[1];
    if (1 > ar) {
      ii->size[1] = 0;
    } else {
      ii->size[1] = ar;
    }

    emxEnsureCapacity((emxArray__common *)ii, ib, (int)sizeof(int));
  }

  emxFree_boolean_T(&x);
  ib = b2_4ac->size[0] * b2_4ac->size[1];
  b2_4ac->size[0] = 1;
  b2_4ac->size[1] = ii->size[1];
  emxEnsureCapacity((emxArray__common *)b2_4ac, ib, (int)sizeof(double));
  cr = ii->size[0] * ii->size[1];
  for (ib = 0; ib < cr; ib++) {
    b2_4ac->data[ib] = ii->data[ib];
  }

  emxFree_int32_T(&ii);
  if (!(b2_4ac->size[1] == 0)) {
    emxInit_real_T(&c_a, 2);
    ib = c_a->size[0] * c_a->size[1];
    c_a->size[0] = b2_4ac->size[1];
    c_a->size[1] = 3;
    emxEnsureCapacity((emxArray__common *)c_a, ib, (int)sizeof(double));
    for (ib = 0; ib < 3; ib++) {
      cr = b2_4ac->size[1];
      for (br = 0; br < cr; br++) {
        c_a->data[br + c_a->size[0] * ib] = V->data[ib + V->size[0] * ((int)
          b2_4ac->data[b2_4ac->size[0] * br] - 1)];
      }
    }

    b_emxInit_real_T(&b_y, 1);
    unnamed_idx_1 = (unsigned int)c_a->size[0];
    b_n = c_a->size[0];
    ib = b_y->size[0];
    b_y->size[0] = (int)unnamed_idx_1;
    emxEnsureCapacity((emxArray__common *)b_y, ib, (int)sizeof(double));
    cr = (int)unnamed_idx_1;
    for (ib = 0; ib < cr; ib++) {
      b_y->data[ib] = 0.0;
    }

    cr = 0;
    while (cr <= 0) {
      for (ic = 1; ic <= b_n; ic++) {
        b_y->data[ic - 1] = 0.0;
      }

      cr = b_n;
    }

    br = 0;
    cr = 0;
    while (cr <= 0) {
      ar = 0;
      for (ib = br; ib + 1 <= br + 3; ib++) {
        if (C_S[ib] != 0.0) {
          ia = ar;
          for (ic = 0; ic + 1 <= b_n; ic++) {
            ia++;
            b_y->data[ic] += C_S[ib] * c_a->data[ia - 1];
          }
        }

        ar += b_n;
      }

      br += 3;
      cr = b_n;
    }

    emxFree_real_T(&c_a);
    emxInit_real_T(&c_y, 2);
    ib = c_y->size[0] * c_y->size[1];
    c_y->size[0] = 1;
    c_y->size[1] = b_y->size[0];
    emxEnsureCapacity((emxArray__common *)c_y, ib, (int)sizeof(double));
    cr = b_y->size[0];
    for (ib = 0; ib < cr; ib++) {
      c_y->data[c_y->size[0] * ib] = -b_y->data[ib];
    }

    emxFree_real_T(&b_y);
    emxInit_real_T(&d_a, 2);
    ib = d_a->size[0] * d_a->size[1];
    d_a->size[0] = 1;
    d_a->size[1] = b2_4ac->size[1];
    emxEnsureCapacity((emxArray__common *)d_a, ib, (int)sizeof(double));
    cr = b2_4ac->size[0] * b2_4ac->size[1];
    for (ib = 0; ib < cr; ib++) {
      d_a->data[ib] = a->data[(int)b2_4ac->data[ib] - 1];
    }

    b_rdivide(c_y, d_a, r0);
    cr = r0->size[0] * r0->size[1];
    emxFree_real_T(&d_a);
    emxFree_real_T(&c_y);
    for (ib = 0; ib < cr; ib++) {
      lambda->data[(int)b2_4ac->data[ib] - 1] = r0->data[ib];
    }

    emxInit_real_T(&e_V, 2);
    ib = e_V->size[0] * e_V->size[1];
    e_V->size[0] = 3;
    e_V->size[1] = b2_4ac->size[1];
    emxEnsureCapacity((emxArray__common *)e_V, ib, (int)sizeof(double));
    cr = b2_4ac->size[1];
    for (ib = 0; ib < cr; ib++) {
      for (br = 0; br < 3; br++) {
        e_V->data[br + e_V->size[0] * ib] = V->data[br + V->size[0] * ((int)
          b2_4ac->data[b2_4ac->size[0] * ib] - 1)];
      }
    }

    emxInit_real_T(&b_lambda, 2);
    ib = b_lambda->size[0] * b_lambda->size[1];
    b_lambda->size[0] = 1;
    b_lambda->size[1] = b2_4ac->size[1];
    emxEnsureCapacity((emxArray__common *)b_lambda, ib, (int)sizeof(double));
    cr = b2_4ac->size[0] * b2_4ac->size[1];
    for (ib = 0; ib < cr; ib++) {
      b_lambda->data[ib] = lambda->data[(int)b2_4ac->data[ib] - 1];
    }

    bsxfun(e_V, b_lambda, temp);
    b_bsxfun(temp, b_C, V);
    cr = V->size[1];
    emxFree_real_T(&b_lambda);
    emxFree_real_T(&e_V);
    for (ib = 0; ib < cr; ib++) {
      for (br = 0; br < 3; br++) {
        X->data[br + X->size[0] * ((int)b2_4ac->data[b2_4ac->size[0] * ib] - 1)]
          = V->data[br + V->size[0] * ib];
      }
    }

    //  C + V*lambda: vector 3 by N
  }

  emxFree_real_T(&temp);
  emxFree_real_T(&V);
  emxInit_real_T(&b_X, 2);

  //  camera projection
  cr = X->size[1];
  ib = b_X->size[0] * b_X->size[1];
  b_X->size[0] = 1;
  b_X->size[1] = cr;
  emxEnsureCapacity((emxArray__common *)b_X, ib, (int)sizeof(double));
  for (ib = 0; ib < cr; ib++) {
    b_X->data[b_X->size[0] * ib] = X->data[X->size[0] * ib];
  }

  emxInit_real_T(&c_X, 2);
  cr = X->size[1];
  ib = c_X->size[0] * c_X->size[1];
  c_X->size[0] = 1;
  c_X->size[1] = cr;
  emxEnsureCapacity((emxArray__common *)c_X, ib, (int)sizeof(double));
  for (ib = 0; ib < cr; ib++) {
    c_X->data[c_X->size[0] * ib] = X->data[2 + X->size[0] * ib];
  }

  emxInit_real_T(&x1, 2);
  emxInit_real_T(&d_X, 2);
  b_rdivide(b_X, c_X, x1);

  //  vector: 1 by N
  cr = X->size[1];
  ib = d_X->size[0] * d_X->size[1];
  d_X->size[0] = 1;
  d_X->size[1] = cr;
  emxEnsureCapacity((emxArray__common *)d_X, ib, (int)sizeof(double));
  emxFree_real_T(&c_X);
  emxFree_real_T(&b_X);
  for (ib = 0; ib < cr; ib++) {
    d_X->data[d_X->size[0] * ib] = X->data[1 + X->size[0] * ib];
  }

  emxInit_real_T(&e_X, 2);
  cr = X->size[1];
  ib = e_X->size[0] * e_X->size[1];
  e_X->size[0] = 1;
  e_X->size[1] = cr;
  emxEnsureCapacity((emxArray__common *)e_X, ib, (int)sizeof(double));
  for (ib = 0; ib < cr; ib++) {
    e_X->data[e_X->size[0] * ib] = X->data[2 + X->size[0] * ib];
  }

  emxInit_real_T(&b_y1, 2);
  emxInit_real_T(&r2, 2);
  b_rdivide(d_X, e_X, b_y1);

  //  vector: 1 by N
  power(x1, r2);
  power(b_y1, r0);
  ib = r2->size[0] * r2->size[1];
  r2->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)r2, ib, (int)sizeof(double));
  br = r2->size[0];
  cr = r2->size[1];
  cr *= br;
  emxFree_real_T(&e_X);
  emxFree_real_T(&d_X);
  for (ib = 0; ib < cr; ib++) {
    r2->data[ib] += r0->data[ib];
  }

  emxFree_real_T(&r0);

  //  vector: 1 by N
  power(r2, b2_4ac);

  //  vector: 1 by N
  ib = lambda->size[0] * lambda->size[1];
  lambda->size[0] = 1;
  lambda->size[1] = b2_4ac->size[1];
  emxEnsureCapacity((emxArray__common *)lambda, ib, (int)sizeof(double));
  cr = b2_4ac->size[0] * b2_4ac->size[1];
  for (ib = 0; ib < cr; ib++) {
    lambda->data[ib] = b2_4ac->data[ib] * r2->data[ib];
  }

  emxInit_real_T(&c_p, 2);

  //  vector: 1 by N
  b_a = 2.0 * p[21];
  power(x1, b);

  //  vector: 1 by N
  e_a = 2.0 * p[22];
  power(b_y1, a);

  //  vector: 1 by N
  //  vector: 1 by N
  //  vector: 1 by N
  //  2 by N
  // % camera observations x_cam %%
  // global x_cam;
  // str = xmlfile.Children(4).Children(8).Children.Data;
  // data = str2num(regexprep(str,'\r\n|\n|\r',''));
  // pt1 = data(:);
  // x_cam = reshape(pt1,2,length(pt1)/2); % 2 by N
  // % compute error %%
  ib = c_p->size[0] * c_p->size[1];
  c_p->size[0] = 2;
  c_p->size[1] = x1->size[1];
  emxEnsureCapacity((emxArray__common *)c_p, ib, (int)sizeof(double));
  cr = x1->size[1];
  for (ib = 0; ib < cr; ib++) {
    c_p->data[c_p->size[0] * ib] = p[14] * ((x1->data[x1->size[0] * ib] * (((1.0
      + p[18] * r2->data[r2->size[0] * ib]) + p[19] * b2_4ac->data[b2_4ac->size
      [0] * ib]) + p[20] * lambda->data[lambda->size[0] * ib]) + b_a * x1->
      data[x1->size[0] * ib] * b_y1->data[b_y1->size[0] * ib]) + p[22] *
      (r2->data[r2->size[0] * ib] + 2.0 * b->data[b->size[0] * ib])) + p[16];
  }

  emxFree_real_T(&b);
  cr = b_y1->size[1];
  for (ib = 0; ib < cr; ib++) {
    c_p->data[1 + c_p->size[0] * ib] = p[15] * ((b_y1->data[b_y1->size[0] * ib] *
      (((1.0 + p[18] * r2->data[r2->size[0] * ib]) + p[19] * b2_4ac->data
        [b2_4ac->size[0] * ib]) + p[20] * lambda->data[lambda->size[0] * ib]) +
      e_a * x1->data[x1->size[0] * ib] * b_y1->data[b_y1->size[0] * ib]) + p[21]
      * (r2->data[r2->size[0] * ib] + 2.0 * a->data[a->size[0] * ib])) + p[17];
  }

  emxFree_real_T(&r2);
  emxFree_real_T(&b_y1);
  emxFree_real_T(&x1);
  emxFree_real_T(&lambda);
  emxFree_real_T(&b2_4ac);
  emxFree_real_T(&a);
  emxInit_real_T(&b_err, 2);
  ib = b_err->size[0] * b_err->size[1];
  b_err->size[0] = 2;
  b_err->size[1] = c_p->size[1];
  emxEnsureCapacity((emxArray__common *)b_err, ib, (int)sizeof(double));
  cr = c_p->size[1];
  for (ib = 0; ib < cr; ib++) {
    for (br = 0; br < 2; br++) {
      b_err->data[br + b_err->size[0] * ib] = c_p->data[br + c_p->size[0] * ib]
        - x_cam->data[br + x_cam->size[0] * ib];
    }
  }

  emxFree_real_T(&c_p);
//  ib = err->size[0];
  err->size[0] = b_err->size[1] << 1;
  ib = err->size[0];
  emxEnsureCapacity((emxArray__common *)err, ib, (int)sizeof(double));
  cr = b_err->size[1] << 1;
  for (ib = 0; ib < cr; ib++) {
    err->data[ib] = b_err->data[ib];
  }

  emxFree_real_T(&b_err);

  //  output: 2N by 1
  // % compute jacobian %%
//  ib = A->size[0] * A->size[1];
  A->size[0] = (int)(2.0 * (double)n);
  A->size[1] = 10;
  ib = A->size[0] * A->size[1];
  emxEnsureCapacity((emxArray__common *)A, ib, (int)sizeof(double));
  cr = (int)(2.0 * (double)n) * 10;
  for (ib = 0; ib < cr; ib++) {
    A->data[ib] = 0.0;
  }

  //  projector intrinsic and extrinsic
//  ib = B->size[0] * B->size[1];
  B->size[0] = (int)(2.0 * (double)n);
  B->size[1] = 4;
  ib = B->size[0] * B->size[1];
  emxEnsureCapacity((emxArray__common *)B, ib, (int)sizeof(double));
  cr = (int)(2.0 * (double)n) << 2;
  for (ib = 0; ib < cr; ib++) {
    B->data[ib] = 0.0;
  }

  //  sphere pose
//  ib = C->size[0] * C->size[1];
  C->size[0] = (int)(2.0 * (double)n);
  C->size[1] = 9;
  ib = C->size[0] * C->size[1];
  emxEnsureCapacity((emxArray__common *)C, ib, (int)sizeof(double));
  cr = (int)(2.0 * (double)n) * 9;
  for (ib = 0; ib < cr; ib++) {
    C->data[ib] = 0.0;
  }

  //  camera intrinsics (with lens distortions)
  fx_p = p[0];
  fy_p = p[1];
  cx_p = p[2];
  cy_p = p[3];
  r11 = R[0];
  r12 = R[3];
  r13 = R[6];
  r21 = R[1];
  r22 = R[4];
  r23 = R[7];
  r31 = R[2];
  r32 = R[5];
  r33 = R[8];
  t1 = p[7];
  t2 = p[8];
  t3 = p[9];
  s1 = p[10];
  s2 = p[11];
  s3 = p[12];
  fx_c = p[14];
  fy_c = p[15];
  kc1 = p[18];
  kc2 = p[19];
  kc3 = p[20];
  pc1 = p[21];
  pc2 = p[22];
  for (ar = 0; ar < n; ar++) {
    JdxdX(X->data[X->size[0] * ar], X->data[1 + X->size[0] * ar], X->data[2 +
          X->size[0] * ar], fx_c, fy_c, kc1, kc2, kc3, pc1, pc2, J_dxdX);

    // JDXDFC
    //     JDXDFC = JDXDFC(X1,X2,X3,FX_C,FY_C,CX_C,CY_C,KC1,KC2,KC3,PC1,PC2)
    //     This function was generated by the Symbolic Math Toolbox version 6.2. 
    //     11-Aug-2016 17:44:09
    b_t2 = 1.0 / (X->data[2 + X->size[0] * ar] * X->data[2 + X->size[0] * ar]);
    b_t3 = X->data[X->size[0] * ar] * X->data[X->size[0] * ar];
    t4 = X->data[1 + X->size[0] * ar] * X->data[1 + X->size[0] * ar];
    t5 = b_t2 * t4;
    t7 = b_t2 * b_t3;
    t6 = t5 + t7;
    t8 = t6 * t6;
    t9 = 1.0 / X->data[2 + X->size[0] * ar];
    t13 = ((kc2 * t8 + kc3 * t6 * t8) + kc1 * t6) + 1.0;
    b_x[0] = (pc2 * (t5 + b_t2 * b_t3 * 3.0) + X->data[X->size[0] * ar] * t9 *
              t13) + X->data[X->size[0] * ar] * X->data[1 + X->size[0] * ar] *
      pc1 * b_t2 * 2.0;
    b_x[1] = 0.0;
    b_x[2] = 0.0;
    b_x[3] = (pc1 * (t7 + b_t2 * t4 * 3.0) + X->data[1 + X->size[0] * ar] * t9 *
              t13) + X->data[X->size[0] * ar] * X->data[1 + X->size[0] * ar] *
      pc2 * b_t2 * 2.0;
    for (br = 0; br < 4; br++) {
      J_dxdfc[br] = b_x[br];
    }

    // JDXDKC
    //     JDXDKC = JDXDKC(X1,X2,X3,FX_C,FY_C,CX_C,CY_C,KC1,KC2,KC3,PC1,PC2)
    //     This function was generated by the Symbolic Math Toolbox version 6.2. 
    //     11-Aug-2016 17:44:09
    b_t2 = 1.0 / (X->data[2 + X->size[0] * ar] * X->data[2 + X->size[0] * ar]);
    b_t3 = 1.0 / X->data[2 + X->size[0] * ar];
    t4 = X->data[X->size[0] * ar] * X->data[X->size[0] * ar];
    t5 = b_t2 * t4;
    t6 = X->data[1 + X->size[0] * ar] * X->data[1 + X->size[0] * ar];
    t7 = b_t2 * t6;
    t8 = t5 + t7;
    t9 = t8 * t8;
    c_x[0] = X->data[X->size[0] * ar] * fx_c * b_t3 * t8;
    c_x[1] = X->data[1 + X->size[0] * ar] * fy_c * b_t3 * t8;
    c_x[2] = X->data[X->size[0] * ar] * fx_c * b_t3 * t9;
    c_x[3] = X->data[1 + X->size[0] * ar] * fy_c * b_t3 * t9;
    c_x[4] = X->data[X->size[0] * ar] * fx_c * b_t3 * t8 * t9;
    c_x[5] = X->data[1 + X->size[0] * ar] * fy_c * b_t3 * t8 * t9;
    c_x[6] = X->data[X->size[0] * ar] * X->data[1 + X->size[0] * ar] * fx_c *
      b_t2 * 2.0;
    c_x[7] = fy_c * (t5 + b_t2 * t6 * 3.0);
    c_x[8] = fx_c * (t7 + b_t2 * t4 * 3.0);
    c_x[9] = X->data[X->size[0] * ar] * X->data[1 + X->size[0] * ar] * fy_c *
      b_t2 * 2.0;
    memcpy(&J_dxdkc[0], &c_x[0], 10U * sizeof(double));
    JdXdfp(fx_p, fy_p, cx_p, cy_p, r11, r21, r31, r12, r22, r32, r13, r23, r33,
           t1, t2, t3, s1, s2, s3, r, x_proj->data[x_proj->size[0] * ar],
           x_proj->data[1 + x_proj->size[0] * ar], dv0);
    JdXdcp(fx_p, fy_p, cx_p, cy_p, r11, r21, r31, r12, r22, r32, r13, r23, r33,
           t1, t2, t3, s1, s2, s3, r, x_proj->data[x_proj->size[0] * ar],
           x_proj->data[1 + x_proj->size[0] * ar], dv1);
    JdXdR(fx_p, fy_p, cx_p, cy_p, r11, r21, r31, r12, r22, r32, r13, r23, r33,
          t1, t2, t3, s1, s2, s3, r, x_proj->data[x_proj->size[0] * ar],
          x_proj->data[1 + x_proj->size[0] * ar], dv2);
    JdXdT(fx_p, fy_p, cx_p, cy_p, r11, r21, r31, r12, r22, r32, r13, r23, r33,
          t1, t2, t3, s1, s2, s3, r, x_proj->data[x_proj->size[0] * ar],
          x_proj->data[1 + x_proj->size[0] * ar], R);
    b_a = 2.0 * (1.0 + (double)ar);
    for (ib = 0; ib < 2; ib++) {
      for (br = 0; br < 9; br++) {
        b_J_dxdX[ib + (br << 1)] = 0.0;
        for (cr = 0; cr < 3; cr++) {
          b_J_dxdX[ib + (br << 1)] += J_dxdX[ib + (cr << 1)] * dv2[cr + 3 * br];
        }
      }

      for (br = 0; br < 2; br++) {
        c_J_dxdX[ib + (br << 1)] = 0.0;
        for (cr = 0; cr < 3; cr++) {
          c_J_dxdX[ib + (br << 1)] += J_dxdX[ib + (cr << 1)] * dv0[cr + 3 * br];
        }

        d_J_dxdX[ib + (br << 1)] = 0.0;
        for (cr = 0; cr < 3; cr++) {
          d_J_dxdX[ib + (br << 1)] += J_dxdX[ib + (cr << 1)] * dv1[cr + 3 * br];
        }
      }

      for (br = 0; br < 3; br++) {
        e_J_dxdX[ib + (br << 1)] = 0.0;
        for (cr = 0; cr < 9; cr++) {
          e_J_dxdX[ib + (br << 1)] += b_J_dxdX[ib + (cr << 1)] * J_dRdom[cr + 9 *
            br];
        }

        f_J_dxdX[ib + (br << 1)] = 0.0;
        for (cr = 0; cr < 3; cr++) {
          f_J_dxdX[ib + (br << 1)] += J_dxdX[ib + (cr << 1)] * R[cr + 3 * br];
        }
      }
    }

    for (ib = 0; ib < 2; ib++) {
      for (br = 0; br < 2; br++) {
        A->data[((int)(b_a + (-1.0 + (double)br)) + A->size[0] * ib) - 1] =
          c_J_dxdX[br + (ib << 1)];
      }
    }

    for (ib = 0; ib < 2; ib++) {
      for (br = 0; br < 2; br++) {
        A->data[((int)(b_a + (-1.0 + (double)br)) + A->size[0] * (ib + 2)) - 1] =
          d_J_dxdX[br + (ib << 1)];
      }
    }

    for (ib = 0; ib < 3; ib++) {
      for (br = 0; br < 2; br++) {
        A->data[((int)(b_a + (-1.0 + (double)br)) + A->size[0] * (ib + 4)) - 1] =
          e_J_dxdX[br + (ib << 1)];
      }
    }

    for (ib = 0; ib < 3; ib++) {
      for (br = 0; br < 2; br++) {
        A->data[((int)(b_a + (-1.0 + (double)br)) + A->size[0] * (ib + 7)) - 1] =
          f_J_dxdX[br + (ib << 1)];
      }
    }

    JdXdS(fx_p, fy_p, cx_p, cy_p, r11, r21, r31, r12, r22, r32, r13, r23, r33,
          t1, t2, t3, s1, s2, s3, r, x_proj->data[x_proj->size[0] * ar],
          x_proj->data[1 + x_proj->size[0] * ar], dv3);
    b_a = 2.0 * (1.0 + (double)ar);
    for (ib = 0; ib < 2; ib++) {
      for (br = 0; br < 4; br++) {
        B->data[((int)(b_a + (-1.0 + (double)ib)) + B->size[0] * br) - 1] = 0.0;
        for (cr = 0; cr < 3; cr++) {
          B->data[((int)(b_a + (-1.0 + (double)ib)) + B->size[0] * br) - 1] +=
            J_dxdX[ib + (cr << 1)] * dv3[cr + 3 * br];
        }
      }
    }

    b_a = 2.0 * (1.0 + (double)ar);
    for (ib = 0; ib < 2; ib++) {
      for (br = 0; br < 2; br++) {
        C->data[((int)(b_a + (-1.0 + (double)br)) + C->size[0] * ib) - 1] =
          J_dxdfc[br + (ib << 1)];
      }
    }

    for (ib = 0; ib < 2; ib++) {
      for (br = 0; br < 2; br++) {
        C->data[((int)(b_a + (-1.0 + (double)br)) + C->size[0] * (ib + 2)) - 1] =
          J_dxdcc[br + (ib << 1)];
      }
    }

    for (ib = 0; ib < 5; ib++) {
      for (br = 0; br < 2; br++) {
        C->data[((int)(b_a + (-1.0 + (double)br)) + C->size[0] * (ib + 4)) - 1] =
          J_dxdkc[br + (ib << 1)];
      }
    }
  }

  emxFree_real_T(&X);
}

//
// File trailer for func_FC.cpp
//
// [EOF]
//
