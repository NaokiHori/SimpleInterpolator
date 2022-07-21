#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "simple_npyio.h"
#include "io.h"


int load_from_npy(const char dirname[], const char varname[], const size_t size, const size_t nitems, void *data){
  /* additional characters: "/", ".npy", NUL, 6 in total */
  char *fname = calloc(strlen(dirname)+strlen(varname)+6, sizeof(char));
  sprintf(fname, "%s/%s.npy", dirname, varname);
  FILE *fp = fopen(fname, "r");
  if(fp == NULL){
    fprintf(stderr, "file open error: %s\n", fname);
    exit(EXIT_FAILURE);
  }
  size_t ndim;
  size_t *shape = NULL;
  char *dtype = NULL;
  bool is_fortran_order;
  size_t header_size = simple_npyio_r_header(&ndim, &shape, &dtype, &is_fortran_order, fp);
  if(header_size == 0){
    fprintf(stderr, "%s:%d npyio header read failed\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }
  int retval = fread(data, size, nitems, fp);
  if(retval != (int)(nitems)){
    fprintf(stderr, "fread error\n");
    exit(EXIT_FAILURE);
  }
  fclose(fp);
  free(shape);
  free(dtype);
  return 0;
}

int save_to_npy(const char fname[], const size_t ndim, const size_t *shape, const char dtype[], const bool is_fortran_order, const size_t size, const void *data){
  FILE *fp = fopen(fname, "w");
  if(fp == NULL){
    fprintf(stderr, "file open error: %s\n", fname);
    exit(EXIT_FAILURE);
  }
  size_t header_size = simple_npyio_w_header(ndim, shape, dtype, is_fortran_order, fp);
  if(header_size == 0){
    fprintf(stderr, "%s:%d npyio header write failed\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }
  size_t nitems = 1;
  for(size_t n = 0; n < ndim; n++){
    nitems *= shape[n];
  }
  int retval = fwrite(data, size, nitems, fp);
  if(retval != (int)(nitems)){
    fprintf(stderr, "fwrite error\n");
    exit(EXIT_FAILURE);
  }
  fclose(fp);
  return 0;
}

