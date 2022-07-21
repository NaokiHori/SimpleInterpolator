#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>
#include <assert.h>
#include "halo.h"
#include "interpolate.h"


static int simple_interpolation_fft(const int n1, const int n2, const double *y1s, double *y2s){
  /*
   * Interpolate values in y1s to y2s using FFT.
   * For instance, original dataset y1s, whose size "n1" is 8:
   *                 z4+z4*
   * z0, z1, z2, z3, ------, z3*, z2*, z1*
   *                   2
   * ^^  ^^  ^^  ^^  ^^^^^^
   * is interpolated to a refined dataset y2s, whose size "n2" is 12:
   *                 z4           z4*
   * z0, z1, z2, z3, --, 0, 0, 0, ---, z3*, z2*, z1*
   *                  2            2
   * ^^  ^^  ^^  ^^  ^^  ^  ^
   * in spectral space (z is Fourier transform of y).
   * Note that asterisks denote the complex conjugate, and z4 = z4* holds (i.e., imag(z4) = 0) in the above example since y1s is a real array.
   * In practice, first n1/2+1 and n2/1+1 elements (underlined) are saved in memory respectively because of the redundancy.
   * NOTE: lengths of y1s and y2s should be even for now
   */
  // check arrays have even sizes
  assert(n1 % 2 == 0);
  assert(n2 % 2 == 0);
  // array sizes in spectral space
  int n1_s = n1/2+1;
  int n2_s = n2/2+1;
  // allocate memories to store spectral data
  fftw_complex *z1s = NULL;
  fftw_complex *z2s = NULL;
  z1s = calloc(n1_s, sizeof(fftw_complex));
  z2s = calloc(n2_s, sizeof(fftw_complex));
  // zero initialisation (just in case)
  memset(z1s, 0, sizeof(fftw_complex)*n1_s);
  memset(z2s, 0, sizeof(fftw_complex)*n2_s);
  // FFTW planner accepts double pointer which is not with const.
  // Thus copying the original array to a buffer
  double *buf = NULL;
  buf = calloc(n1, sizeof(double));
  memcpy(buf, y1s, sizeof(double)*n1);
  // create plan (FFTW_ESTIMATE or other planner flag which do not destroy input arrays should be used)
  fftw_plan fwrd = fftw_plan_dft_r2c_1d(n1, buf, z1s, FFTW_ESTIMATE);
  fftw_plan bwrd = fftw_plan_dft_c2r_1d(n2, z2s, y2s, FFTW_ESTIMATE);
  // project original physical array to spectral space
  fftw_execute(fwrd);
  // assign positive wavenumbers
  for(int i=0; i<n1/2+1; i++){
    // (n1/2-1)-th element should be halved
    double factor = i == n1/2 ? 0.5 : 1.0;
    z2s[i] = factor*z1s[i];
  }
  // project refined array to physical space
  fftw_execute(bwrd);
  // normalise
  for(int i=0; i<n2; i++){
    y2s[i] /= 1.*n1;
  }
  // clean-up
  fftw_destroy_plan(fwrd);
  fftw_destroy_plan(bwrd);
  free(z1s);
  free(z2s);
  free(buf);
  return 0;
}

typedef struct cubic_coefs_t_ {
  // coef3 x^3 + coef2 x^2 + coef1 x^1 + coef0
  double coef3;
  double coef2;
  double coef1;
  double coef0;
} cubic_coefs_t;

static int set_cubic_coefs(const double xs[4], const double ys[4], cubic_coefs_t *st){
  // project global coordinate to local coordinate
  // xs[0] -> xmm
  // xs[1] -> xm  ( = -1 )
  // xs[2] -> xp  ( = +1 )
  // xs[3] -> xpp
  double xm  = -1.;
  double xp  = +1.;
  double xmm = (xp-xm)/(xs[2]-xs[1])*(xs[0]-xs[1])+xm;
  double xpp = (xp-xm)/(xs[2]-xs[1])*(xs[3]-xs[1])+xm;
  // y is identical, but use the same notations as x
  double ymm = ys[0];
  double ym  = ys[1];
  double yp  = ys[2];
  double ypp = ys[3];
  if(xs[0] == xs[1]){
    // left boundary, quadratic function is fitted
    // solve equation w.r.t. coef0
    // 2 (x^2-1) "coef0" = (yp+ym) x^2 + (yp-ym) x - 2y
    st->coef0 = ((yp+ym)*pow(xpp, 2.)+(yp-ym)*xpp-2.*ypp)/(2.*(pow(xpp, 2.)-1.));
    st->coef1 = 0.5*(yp-ym);
    st->coef2 = -st->coef0+0.5*(yp+ym);
    st->coef3 = 0.;
  }else if(xs[2] == xs[3]){
    // right boundary, quadratic function is fitted
    // solve equation w.r.t. coef0
    // 2 (x^2-1) "coef0" = (yp+ym) x^2 + (yp-ym) x - 2y
    st->coef0 = ((yp+ym)*pow(xmm, 2.)+(yp-ym)*xmm-2.*ymm)/(2.*(pow(xmm, 2.)-1.));
    st->coef1 = 0.5*(yp-ym);
    st->coef2 = -st->coef0+0.5*(yp+ym);
    st->coef3 = 0.;
  }else{
    // middle, cubic function is fitted
    // solve 2x2 linear system w.r.t. coef0 and coef1
    // 2 (x^2-1) "coef0" + 2 x (x^2-1) "coef1" = (yp-ym) x^3 + (yp+ym) x^2 - 2y
    double b0 = (yp-ym)*pow(xmm, 3.)+(yp+ym)*pow(xmm, 2.)-2.*ymm;
    double b1 = (yp-ym)*pow(xpp, 3.)+(yp+ym)*pow(xpp, 2.)-2.*ypp;
    double a00 = 2.    *(pow(xmm, 2.)-1.);
    double a01 = 2.*xmm*(pow(xmm, 2.)-1.);
    double a10 = 2.    *(pow(xpp, 2.)-1.);
    double a11 = 2.*xpp*(pow(xpp, 2.)-1.);
    double det = a00*a11-a01*a10;
    st->coef0 = 1./det*(+a11*b0-a01*b1);
    st->coef1 = 1./det*(-a10*b0+a00*b1);
    st->coef2 = -st->coef0+0.5*(yp+ym);
    st->coef3 = -st->coef1+0.5*(yp-ym);
  }
  return 0;
}

static int simple_interpolation_cubic(const int n1, const int n2, const double *x1s, const double *x2s, const double *y1s, double *y2s){
  cubic_coefs_t *coefs = NULL;
  // compute coefficients of piecewise cubic (quadratic) polynomial for each interval (x1s[i], x1s[i+1])
  coefs = calloc(n1-1, sizeof(cubic_coefs_t));
  for(int i=0; i<n1-1; i++){
    double localxs[4];
    double localys[4];
    localxs[0] = i ==    0 ? x1s[i  ] : x1s[i-1];
    localxs[1] =                        x1s[i  ];
    localxs[2] =                        x1s[i+1];
    localxs[3] = i == n1-2 ? x1s[i+1] : x1s[i+2];
    localys[0] = i ==    0 ? y1s[i  ] : y1s[i-1];
    localys[1] =                        y1s[i  ];
    localys[2] =                        y1s[i+1];
    localys[3] = i == n1-2 ? y1s[i+1] : y1s[i+2];
    set_cubic_coefs(localxs, localys, &(coefs[i]));
  }
  // for each x2s, get corresponding index of (x1s[i], x1s[i+1])
  int *indices = NULL;
  indices = calloc(n2, sizeof(int));
  indices[0] = 0; // left boundary
  for(int i2=1; i2<n2-1; i2++){
    double x2 = x2s[i2];
    int i1 = 0;
    while(1){
      double x1_m = x1s[i1  ];
      double x1_p = x1s[i1+1];
      if(x1_m <= x2 && x2 <= x1_p){
        break;
      }
      i1++;
    }
    indices[i2] = i1;
  }
  indices[n2-1] = n1-2; // right boundary
  // interpolate
  for(int i2=0; i2<n2; i2++){
    // convert x to local coordinate system
    double x = x2s[i2];
    int i1 = indices[i2];
    double xm = x1s[i1  ];
    double xp = x1s[i1+1];
    x = 2./(xp-xm)*(x-xm)-1.;
    // extract coefficients and interpolate
    cubic_coefs_t st = coefs[i1];
    double coef3 = st.coef3;
    double coef2 = st.coef2;
    double coef1 = st.coef1;
    double coef0 = st.coef0;
    double y = coef3*pow(x, 3.)+coef2*pow(x, 2.)+coef1*pow(x, 1.)+coef0;
    y2s[i2] = y;
  }
  // clean-up
  free(indices);
  free(coefs);
  return 0;
}

int interpolate_scalar_potential(const interpolate_t *bef, const interpolate_t *aft, const double *bef_q, double *aft_q){
  const int bef_itot = bef->itot;
  const int bef_jtot = bef->jtot;
  const int aft_itot = aft->itot;
  const int aft_jtot = aft->jtot;
  const double *bef_xf = bef->xf;
  const double *aft_xf = aft->xf;
  // intermediate scalar potential, interpolated only in x
  double *int_q = calloc((aft_itot+1)*bef_jtot, sizeof(double));
  for(int j=1; j<=bef_jtot; j++){
    simple_interpolation_cubic(bef_itot+1, aft_itot+1, bef_xf, aft_xf, bef_q+j*(bef_itot+1), int_q+(j-1)*(aft_itot+1));
  }
  for(int i=0; i<=aft_itot; i++){
    double *bef_1d = calloc(bef_jtot, sizeof(double));
    double *aft_1d = calloc(aft_jtot, sizeof(double));
    for(int j=0; j<bef_jtot; j++){
      bef_1d[j] = int_q[j*(aft_itot+1)+i];
    }
    simple_interpolation_fft(bef_jtot, aft_jtot, bef_1d, aft_1d);
    for(int j=0; j<aft_jtot; j++){
      aft_q[(j+1)*(aft_itot+1)+i] = aft_1d[j];
    }
    free(bef_1d);
    free(aft_1d);
  }
  free(int_q);
  update_halo(aft_jtot, aft_itot+1, aft_q);
  return 0;
}

