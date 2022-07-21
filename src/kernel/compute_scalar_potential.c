#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <float.h>
#include <fftw3.h>
#include "interpolate.h"


#if !defined(M_PI)
#define M_PI 3.1415926535897932
#endif

static int my_zgtsv_b(const int n, double *tdm_l, double *tdm_c, double *tdm_u, fftw_complex *tdm_q){
  tdm_u[0] = tdm_u[0]/tdm_c[0];
  tdm_q[0] = tdm_q[0]/tdm_c[0];
  for(int i=1; i<=n-1; i++){
    double val = tdm_c[i]-tdm_l[i]*tdm_u[i-1];
    if(fabs(val) < DBL_EPSILON){
      tdm_u[i] = 0.;
      tdm_q[i] = 0.;
    }else{
      val = 1./val;
      tdm_u[i] = val* tdm_u[i];
      tdm_q[i] = val*(tdm_q[i]-tdm_l[i]*tdm_q[i-1]);
    }
  }
  for(int i=n-2; i>=0; i--){
    tdm_q[i] -= tdm_u[i]*tdm_q[i+1];
  }
  return 0;
}

int compute_scalar_potential(const interpolate_t *st, double *qrx){
  /* qrx: vorticity (input), scalar potential (output) */
  const int itot = st->itot;
  const int jtot = st->jtot;
  const double *uy = st->uy;
  const double *xf = st->xf;
  const double *yf = st->yf;
  const double dy = (yf[jtot]-yf[0])/jtot;
  /* corrections for moving boundaries */
  // left wall
  for(int j=1; j<=jtot; j++){
    int i = 0;
    double dx = xf[i+1]-xf[i  ];
    double uy_wall = uy[(j  )*(itot+2)+(i  )];
    qrx[(j  )*(itot+1)+(i  )] -= 2.*uy_wall/dx;
  }
  // right wall
  for(int j=1; j<=jtot; j++){
    int i = itot;
    double dx = xf[i  ]-xf[i-1];
    double uy_wall = uy[(j  )*(itot+2)+(i+1)];
    qrx[(j  )*(itot+1)+(i  )] += 2.*uy_wall/dx;
  }
  // solve poisson equation
  /* buffers (different alignments, datatypes) */
  double       *qry = calloc((itot+1)*(jtot    ), sizeof(      double));
  fftw_complex *qcx = calloc((itot+1)*(jtot/2+1), sizeof(fftw_complex));
  fftw_complex *qcy = calloc((itot+1)*(jtot/2+1), sizeof(fftw_complex));
  fftw_plan fwrd = NULL;
  fftw_plan bwrd = NULL;
  /* allocate matrix and create fftw plans */
  {
    fftw_iodim dims[1], hdims[1];
    /* create fftw plans */
    dims[0].n  = jtot;
    dims[0].is = 1;
    dims[0].os = 1;
    // forward transform, real to complex
    hdims[0].n  = itot+1;
    hdims[0].is = jtot;
    hdims[0].os = jtot/2+1;
    fwrd = fftw_plan_guru_dft_r2c(1, dims, 1, hdims, qry, qcy, FFTW_ESTIMATE);
    // backward transform, complex to real
    hdims[0].n  = itot+1;
    hdims[0].is = jtot/2+1;
    hdims[0].os = jtot;
    bwrd = fftw_plan_guru_dft_c2r(1, dims, 1, hdims, qcy, qry, FFTW_ESTIMATE);
  }
  /* transpose real x-aligned matrix to y-aligned matrix */
  for(int j=1; j<=jtot; j++){
    for(int i=0; i<=itot; i++){
      qry[i*jtot+(j-1)] = qrx[(j  )*(itot+1)+(i  )];
    }
  }
  /* project to wave space */
  fftw_execute(fwrd);
  /* transpose complex y-aligned matrix to x-aligned matrix */
  for(int j=0; j<jtot/2+1; j++){
    for(int i=0; i<=itot; i++){
      qcx[j*(itot+1)+i] = qcy[i*(jtot/2+1)+j];
    }
  }
  /* solve linear systems */
  {
    double       *tdm_l = calloc(itot+1, sizeof(      double));
    double       *tdm_c = calloc(itot+1, sizeof(      double));
    double       *tdm_u = calloc(itot+1, sizeof(      double));
    fftw_complex *tdm_q = calloc(itot+1, sizeof(fftw_complex));
    for(int j=0; j<jtot/2+1; j++){
      /* compute eigenvalue of this j position */
      double eigenvalue =
        -4./pow(dy, 2.)
        *pow(sin(M_PI*j/(1.*jtot)), 2.);
      /* set lower and upper diagonal components */
      for(int i=0; i<=itot; i++){
        double dxm = i ==    0 ? xf[   1]-xf[     0] : xf[i  ]-xf[i-1];
        double dxp = i == itot ? xf[itot]-xf[itot-1] : xf[i+1]-xf[i  ];
        double dxc = 0.5*(dxm+dxp);
        tdm_l[i] = 1./dxm/dxc;
        tdm_u[i] = 1./dxp/dxc;
        tdm_c[i] = -tdm_l[i]-tdm_u[i]+eigenvalue;
      }
      tdm_u[   0] += tdm_l[   0];
      tdm_l[itot] += tdm_u[itot];
      /* solve linear system */
      for(int i=0; i<=itot; i++){
        tdm_q[i] = qcx[j*(itot+1)+i];
      }
      my_zgtsv_b(itot+1, tdm_l, tdm_c, tdm_u, tdm_q);
      for(int i=0; i<=itot; i++){
        qcx[j*(itot+1)+i] = tdm_q[i];
      }
    }
    free(tdm_l);
    free(tdm_c);
    free(tdm_u);
    free(tdm_q);
  }
  /* transpose complex x-aligned matrix to y-aligned matrix */
  for(int j=0; j<jtot/2+1; j++){
    for(int i=0; i<=itot; i++){
      qcy[i*(jtot/2+1)+j] = qcx[j*(itot+1)+i];
    }
  }
  /* project to physical space */
  fftw_execute(bwrd);
  /* transpose real y-aligned matrix to x-aligned matrix */
  for(int j=1; j<=jtot; j++){
    for(int i=0; i<=itot; i++){
      qrx[(j  )*(itot+1)+(i  )] = qry[i*jtot+(j-1)];
    }
  }
  /* normalise and store result */
  for(int j=1; j<=jtot; j++){
    for(int i=0; i<=itot; i++){
      qrx[(j  )*(itot+1)+(i  )] /= 1.*jtot;
    }
  }
  /* deallocate memories */
  free(qry);
  free(qcx);
  free(qcy);
  fftw_destroy_plan(fwrd);
  fftw_destroy_plan(bwrd);
  return 0;
}

