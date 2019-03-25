#ifndef NCOMP_UTIL_H
#define NCOMP_UTIL_H
#include "ncomp.h"

void _to_double(void*, size_t, size_t, int, double*);
void _to_float(void*, size_t, size_t, int, float*);

ncomp_array* ncomp_array_alloc(void*, int, int, size_t*);
void         ncomp_array_free(ncomp_array*, int);
#endif
