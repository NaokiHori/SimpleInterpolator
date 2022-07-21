#include "halo.h"


int update_halo(const int jtot, const int xsize, double *data){
  // update halo (ux)
  for(int i=0; i<xsize; i++){
    const int j_f = jtot; // copy from
    const int j_t =    0; // copy to
    data[j_t*xsize+i] = data[j_f*xsize+i];
  }
  for(int i=0; i<xsize; i++){
    const int j_f =      1; // copy from
    const int j_t = jtot+1; // copy to
    data[j_t*xsize+i] = data[j_f*xsize+i];
  }
  return 0;
}

