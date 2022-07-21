#include "halo.h"
#include "interpolate.h"


int compute_velocity(interpolate_t *st, const double *q){
  /* q: scalar potential */
  /* ux = +dq/dy */
  /* uy = -dq/dx */
  const int itot = st->itot;
  const int jtot = st->jtot;
  const double *xf = st->xf;
  const double *yf = st->yf;
  const double dy = (yf[jtot]-yf[0])/jtot;
  double *ux = st->ux;
  double *uy = st->uy;
  for(int j=1; j<=jtot; j++){
    for(int i=1; i<=itot-1; i++){
      double q_ym = q[(j  )*(itot+1)+(i  )];
      double q_yp = q[(j+1)*(itot+1)+(i  )];
      double dq = q_yp-q_ym;
      ux[j*(itot+1)+i] = +dq/dy;
    }
  }
  update_halo(jtot, itot+1, ux);
  for(int j=1; j<=jtot; j++){
    for(int i=1; i<=itot; i++){
      double dx = xf[i  ]-xf[i-1];
      double q_xm = q[(j  )*(itot+1)+(i-1)];
      double q_xp = q[(j  )*(itot+1)+(i  )];
      double dq = q_xp-q_xm;
      uy[j*(itot+2)+i] = -dq/dx;
    }
  }
  update_halo(jtot, itot+2, uy);
  return 0;
}

