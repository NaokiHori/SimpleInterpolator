#include <stdlib.h>      // calloc, free
#include "interpolate.h" // four functions below


int interpolate(const interpolate_t *bef, interpolate_t *aft){
  /* interpolate velocity field contained by "bef" to "aft" */
  // vorticity and scalar potential on the original and interpolated grids
  double *bef_q = calloc((bef->itot+1)*(bef->jtot+2), sizeof(double));
  double *aft_q = calloc((aft->itot+1)*(aft->jtot+2), sizeof(double));
  /* vorticity on the original grid */
  compute_vorticity(bef, bef_q);
  /* scalar potential on the original grid */
  compute_scalar_potential(bef, bef_q);
  /* interpolate scalar potential */
  interpolate_scalar_potential(bef, aft, bef_q, aft_q);
  /* compute velocity */
  compute_velocity(aft, aft_q);
  /* clean-up buffers */
  free(bef_q);
  free(aft_q);
  return 0;
}

