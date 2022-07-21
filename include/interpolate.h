#if !defined(INTERPOLATE_H)
#define INTERPOLATE_H

typedef struct interpolate_t_ {
  // number of pressure node in x (excl. boundary points)
  int itot;
  // number of pressure node in y (excl. halo points)
  int jtot;
  // x cell faces
  // [0:itot]
  double *xf;
  // y cell faces
  // [1:jtot+1]
  // NOTE: this is defined to be consistent, dy is only used
  double *yf;
  // velocity in x
  // [0:jtot+1, 0:itot]
  double *ux;
  // velocity in y
  // [0:jtot+1, 0:itot+1]
  double *uy;
} interpolate_t;

// main routine of div-free interpolation
extern int interpolate(const interpolate_t *bef, interpolate_t *aft);

// each step consisting of div-free interpolation defined under "src/kernel"
extern int compute_vorticity(const interpolate_t *st, double *omega_z);
extern int compute_scalar_potential(const interpolate_t *st, double *qrx);
extern int interpolate_scalar_potential(const interpolate_t *bef, const interpolate_t *aft, const double *bef_q, double *aft_q);
extern int compute_velocity(interpolate_t *st, const double *q);

#endif // INTERPOLATE_H
