#include "interpolate.h"


int compute_vorticity(const interpolate_t *st, double *omega_z){
  /* z-vorticity = -duydx + duxdy */
  const int itot = st->itot;
  const int jtot = st->jtot;
  const double *ux = st->ux;
  const double *uy = st->uy;
  const double *xf = st->xf;
  const double *yf = st->yf;
  const double dy = (yf[jtot]-yf[0])/jtot;
  for(int j=1; j<=jtot; j++){
    for(int i=0; i<=itot; i++){
      // dx
      double xm = i ==    0 ? xf[   0] : 0.5*(xf[i-1]+xf[i  ]);
      double xp = i == itot ? xf[itot] : 0.5*(xf[i  ]+xf[i+1]);
      double dx = xp-xm;
      // dux
      double ux_ym = ux[(j-1)*(itot+1)+(i  )];
      double ux_yp = ux[(j  )*(itot+1)+(i  )];
      double dux = ux_yp-ux_ym;
      // duy
      double uy_xm = uy[(j  )*(itot+2)+(i  )];
      double uy_xp = uy[(j  )*(itot+2)+(i+1)];
      double duy = uy_xp-uy_xm;
      // vorticity
      double duydx = duy/dx;
      double duxdy = dux/dy;
      omega_z[j*(itot+1)+i] = -duydx+duxdy;
    }
  }
  return 0;
}

