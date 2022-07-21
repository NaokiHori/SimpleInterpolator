#if !defined(IO_H)
#define IO_H

#include <stdbool.h>

extern int load_from_npy(const char dirname[], const char varname[], const size_t size, const size_t nitems, void *data);
extern int save_to_npy(const char fname[], const size_t ndim, const size_t *shape, const char dtype[], const bool is_fortran_order, const size_t size, const void *data);

#endif // IO_H
