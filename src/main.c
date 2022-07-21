#include <stdlib.h>      // calloc, free
#include "io.h"          // load_from_npy
#include "halo.h"        // update_halo
#include "interpolate.h" // interpolate_t, interpolate


static interpolate_t *init(const char dirname[], const bool load_vel);
static int clean_up(interpolate_t *st);
static int save_velocity(const interpolate_t *st);

int main(void){
  /* allocate and load original grid / velocity */
  interpolate_t *bef = init("bef", true);
  /* allocate and load target grid, allocate velocity */
  interpolate_t *aft = init("aft", false);
  /* interpolate "bef" velocity to "aft" velocity keeping continuity */
  interpolate(bef, aft);
  /* save interpolated velocity */
  save_velocity(aft);
  /* free all memories */
  clean_up(bef);
  clean_up(aft);
  return 0;
}

static interpolate_t *init(const char dirname[], const bool load_vel){
  /* allocate and init a structure "st" which contains grid and velocity info */
  /* grid info is assumed to exist under a directory "dirname" */
  /* if load_vel is true, also load velocity from the same place */
  interpolate_t *st = calloc(1, sizeof(interpolate_t));
  // number of grid points in each direction (itot, jtot)
  int itot, jtot;
  load_from_npy(dirname, "itot", sizeof(int), 1, &itot);
  load_from_npy(dirname, "jtot", sizeof(int), 1, &jtot);
  st->itot = itot;
  st->jtot = jtot;
  // allocate arrays to store grid points and velocity
  st->xf = calloc(itot+1, sizeof(double));
  st->yf = calloc(jtot+1, sizeof(double));
  st->ux = calloc((itot+1)*(jtot+2), sizeof(double));
  st->uy = calloc((itot+2)*(jtot+2), sizeof(double));
  // load grid points
  load_from_npy(dirname, "xf", sizeof(double), itot+1, st->xf);
  load_from_npy(dirname, "yf", sizeof(double), jtot+1, st->yf);
  // load velocity if desired
  if(load_vel){
    load_from_npy(dirname, "ux", sizeof(double), (itot+1)*jtot, st->ux+(itot+1));
    load_from_npy(dirname, "uy", sizeof(double), (itot+2)*jtot, st->uy+(itot+2));
  }
  // communicate halo cell info
  update_halo(jtot, itot+1, st->ux);
  update_halo(jtot, itot+2, st->uy);
  return st;
}

static int clean_up(interpolate_t *st){
  /* free all memory in interpolate_t */
  free(st->xf);
  free(st->yf);
  free(st->ux);
  free(st->uy);
  free(st);
  return 0;
}

static int save_velocity(const interpolate_t *st){
  /* save ux and uy to files */
  const int itot = st->itot;
  const int jtot = st->jtot;
  const double *ux = st->ux;
  const double *uy = st->uy;
  // NOTE: one row is skipped to exclude halo cells
  size_t shape[2];
  shape[0] = jtot;
  shape[1] = itot+1;
  save_to_npy("aft/ux.npy", 2, shape, "'<f8'", false, sizeof(double), ux+(itot+1));
  shape[0] = jtot;
  shape[1] = itot+2;
  save_to_npy("aft/uy.npy", 2, shape, "'<f8'", false, sizeof(double), uy+(itot+2));
  return 0;
}

