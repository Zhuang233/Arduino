/*
 * File: leogetangle.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 06-Jul-2023 16:22:37
 */

/* Include Files */
#include "leogetangle.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_powd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }
  return y;
}

/*
 * UNTITLED2 此处显示有关此函数的摘要
 *    此处显示详细说明
 *
 * Arguments    : double x
 *                double y
 *                double *theta1
 *                double *theta2
 * Return Type  : void
 */
void leogetangle(double x, double y, double *theta1, double *theta2)
{
  double b_theta1_tmp;
  double theta1_tmp;
  theta1_tmp = x * x;
  b_theta1_tmp = y * y;
  *theta1 =
      2.0 *
      (57.295779513082323 *
       atan((220.0 * y +
             sqrt(((((-rt_powd_snf(x, 4.0) - 2.0 * theta1_tmp * b_theta1_tmp) +
                     44200.0 * theta1_tmp) -
                    rt_powd_snf(y, 4.0)) +
                   44200.0 * b_theta1_tmp) -
                  4.41E+6)) /
            (((theta1_tmp + 220.0 * x) + b_theta1_tmp) + 2100.0)));
  theta1_tmp += b_theta1_tmp;
  *theta2 = -2.0 * (57.295779513082323 *
                    atan(sqrt(-(theta1_tmp - 100.0) * (theta1_tmp - 44100.0)) /
                         (theta1_tmp - 44100.0)));
}

/*
 * File trailer for leogetangle.c
 *
 * [EOF]
 */
