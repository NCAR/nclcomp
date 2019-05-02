#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "ncomp.h"
#include "util.h"

extern void dlinint1_(int *,double *,
                      double *,int *,int *,double *,
                      double *,double *, double *,
                      int *,double *,int *,int *);

int linint1_n_old(ncomp_array* xi, ncomp_array* fi, ncomp_array* xo, ncomp_array* fo, int wrap, double xmsg, int opt, int dim)
{
  double *tmp_xi, *tmp_xo,*tmp_fi, *tmp_fo;
//  int ndims_xi;
//  ng_size_t dsizes_xi[NCL_MAX_DIMENSIONS], dsizes_xo[NCL_MAX_DIMENSIONS];
//  int ndims_fi;
//  ng_size_t dsizes_fi[NCL_MAX_DIMENSIONS];
  int has_missing_fi;
//  size_t *dsizes_fo;
//  NclScalar missing_fi, missing_dfi, missing_rfi, missing_fo;
    double missing_dfi = NC_FILL_DOUBLE;
    float missing_rfi = NC_FILL_FLOAT;
//  int *dim, *opt, iopt = 0;
//  logical *wrap;
//  NclBasicDataTypes type_xi, type_fi, type_xo, type_fo;




    size_t nxi, nxi2, nxo, nfo, nd, nr, nl, nrnxi, nrnxo, ntotal, size_fo;
    int inxi, inxi2, inxo, ier, ret;
    size_t i, j, index_nri, index_nro, index_fi, index_fo;
    double *xiw, *fxiw;

    nxi = fi->shape[dim];
    nxo = xo->shape[0];
    nfo = nxo;
    nxi2 = nxi + 2;

    inxi  = (int) nxi;
    inxo  = (int) nxo;
    inxi2 = (int) nxi2;



/*
 * Some error checking. Make sure input dimension is valid.
 */
  if(dim < 0 || dim >= fi->ndim) {
    printf("linint1_n: Invalid dimension to do interpolation on, can't continue\n");
    return(1);
  }
  if(nxi < 2) {
    printf("linint1_n: xi must have at least 2 elements\n");
    return(2);
  }

/*
 * Test dimension sizes.
 */
  if((nxi > INT_MAX) || (nxo > INT_MAX) || (nxi2 > INT_MAX)) {
    printf("linint1_n: one or more dimension sizes is greater than INT_MAX\n");
    return(3);
  }

/*
 * Check dimensions of xi and fi. If xi is not one-dimensional, then it 
 * must be the same size as fi. Otherwise, the dims-th dimension of
 * fi must be equal to the length of xi.
 */
  if(xi->ndim > 1) {
    if(xi->ndim != fi->ndim) {
      printf("linint1_n: If xi is not one-dimensional, then it must be the same size as fi\n");
      return(4);
    }
    for(i = 0; i < fi->ndim; i++) {
      if(xi->shape[i] != fi->shape[i]) {
        printf("linint1_n: If xi is not one-dimensional, then it must be the same size as fi\n");
        return(5);
      }
    }
  }
  else {
    if(xi->shape[0] != nxi) {
      printf("linint1_n: The dim-th dimension of fi must be the same length as xi\n");
      return(6);
    }
  }


/*
 * Calculate size of leftmost dimensions (nl) up to the dim-th
 *   dimension.
 * Calculate size of rightmost dimensions (nr) from the
 *   dim-th dimension.
 *
 * The dimension to do the interpolation across is "dim".
 */
  nl = nr = 1;
  if(fi->ndim > 1) {
    nd = fi->ndim-1;
    for(i = 0; i < dim ; i++) {
      nl = nl*fi->shape[i];
    }
    for(i = dim+1; i < fi->ndim; i++) {
      nr = nr*fi->shape[i];
    }
  }
  else {
    nd = 1;
  }
  ntotal  = nr * nl;
  size_fo = ntotal * nfo;


/*
 * Coerce missing values.
 */
/*  coerce_missing(fi->type,has_missing_fi,&missing_fi,&missing_dfi, */
  coerce_missing(fi->type,has_missing_fi,&xmsg,&missing_dfi,
                 &missing_rfi);
/*
 * Allocate space for temporary output array.
 */
  tmp_fo = (double*)calloc(nfo,sizeof(double));
  if(tmp_fo == NULL) {
    printf("linint1_n: Unable to allocate memory for temporary arrays\n");
    return(7);
  }

/*
 * Allocate space for output array.
 */
/*  dsizes_fo = (size_t*)calloc(fi->ndim,sizeof(size_t));*/
  if(fi->type == NCOMP_DOUBLE) {
//    fo->addr   = (void*)calloc(size_fo,sizeof(double));
    fo->type   = NCOMP_DOUBLE;
/*    missing_fo = missing_dfi; */
  }
  else {
//    fo->addr   = (void*)calloc(size_fo,sizeof(float));
    fo->type   = NCOMP_FLOAT;
/*    missing_fo = missing_rfi; */
  }
  //if(fo == NULL || dsizes_fo == NULL) {
  if(fo->addr == NULL) {
    printf("linint1_n: Unable to allocate memory for output array\n");
    return(8);
  }
/* 
 * Go ahead and copy all dimesions, but then replace the dim-th one.
 */
/*
  for(i = 0; i < ndims_fi; i++) dsizes_fo[i] = dsizes_fi[i];
  dsizes_fo[*dim] = nxo;
*/

/*
 * Allocate space for work arrays.
 */
  xiw  = (double*)calloc(nxi2,sizeof(double));
  fxiw = (double*)calloc(nxi2,sizeof(double));
  if(xiw == NULL || fxiw == NULL) {
    printf("linint1_n: Unable to allocate memory for work arrays\n");
    return(9);
  }

/*
 * Coerce output array to double if necessary.
 */
  tmp_xo = coerce_input_double(xo->addr,xo->type,nxo,0,NULL,NULL);
  if(tmp_xo == NULL) {
    printf("linint1_n: Unable to coerce output array to double precision\n");
    return(10);
  }

  if(xi->ndim == 1) {
    tmp_xi = coerce_input_double(xi->addr,xi->type,nxi,0,NULL,NULL);
  }
  else {
    tmp_xi = (double*)calloc(nxi,sizeof(double));
    if(tmp_xi == NULL) {
      printf("linint1_n: Unable to allocate memory for coercing input array to double precision\n");
      return(11);
    }
  }

  tmp_fi = (double*)calloc(nxi,sizeof(double));
  if(tmp_fi == NULL) {
    printf("linint1_n: Unable to allocate memory for coercing input array to double precision\n");
    return(12);
  }


/*
 * Loop through leftmost and rightmost dimensions and call Fortran
 * routine for each array subsection.
 */
  nrnxi = nr*nxi;
  nrnxo = nr*nxo;
  for( i = 0; i < nl; i++ ) {
    index_nri = i*nrnxi;
    index_nro = i*nrnxo;
    for( j = 0; j < nr; j++ ) {
      index_fi = index_nri+j;
      index_fo = index_nro+j;

      if(xi->ndim > 1) {
        coerce_subset_input_double_step(xi->addr,tmp_xi,index_fi,nr,xi->type,
                                        nxi,0,NULL,NULL);
      }
      coerce_subset_input_double_step(fi->addr,tmp_fi,index_fi,nr,fi->type,
                                      nxi,0,NULL,NULL);










/*
 * Call Fortran routine.
 */
      dlinint1_(&inxi,tmp_xi,tmp_fi,&wrap,&inxo,tmp_xo,tmp_fo,
                xiw,fxiw,&inxi2,&missing_dfi,&opt,&ier);
/*
printf("tmp_fi[0]: %g\n", tmp_fi[0]);
printf("tmp_fo[0]: %g\n", tmp_fo[0]);
*/
/*                                 xiw,fxiw,&inxi2,&missing_dfi.doubleval, */

      if(ier) {
        printf("linint1_n: xi and xo must be monotonically increasing\n");
        return(7);

        set_subset_output_missing_step(fo->addr,index_fo,nr,fo->type,nfo,
/*                                       missing_dfi.doubleval); */
                                       missing_dfi);
      }
      else {
        coerce_output_float_or_double_step(fo->addr,tmp_fo,fi->type,nfo,index_fo,nr);
/*
printf("tmp_fo[0]: %g\n", tmp_fo[0]);
printf("fo[%d]: %g\n", index_fo, ((double*)fo->addr)[index_fo]);
*/
      }
    }
  }

}

int linint1_n_loop(ncomp_array* xi, ncomp_array* fi, ncomp_array* xo, ncomp_array* fo, double* tmp_xi, double* tmp_fi, double* tmp_fo, double* xiw, double* fxiw, int wrap, double xmsg, int opt, int dim)
{
//  double *tmp_xi, *tmp_xo,*tmp_fi, *tmp_fo;
    double *tmp_xo;
//  int ndims_xi;
//  ng_size_t dsizes_xi[NCL_MAX_DIMENSIONS], dsizes_xo[NCL_MAX_DIMENSIONS];
//  int ndims_fi;
//  ng_size_t dsizes_fi[NCL_MAX_DIMENSIONS];
  int has_missing_fi;
//  size_t *dsizes_fo;
//  NclScalar missing_fi, missing_dfi, missing_rfi, missing_fo;
    double missing_dfi = NC_FILL_DOUBLE;
    float missing_rfi = NC_FILL_FLOAT;
//  int *dim, *opt, iopt = 0;
//  logical *wrap;
//  NclBasicDataTypes type_xi, type_fi, type_xo, type_fo;




    size_t nxi, nxi2, nxo, nfo, nd, nr, nl, nrnxi, nrnxo, ntotal, size_fo;
    int inxi, inxi2, inxo, ier, ret;
    size_t i, j, index_nri, index_nro, index_fi, index_fo;

/*
    for (i=0; i < fi->ndim; i++)
        printf("fi->shape[%d]: %d\n", i, fi->shape[i]);
*/

    nxi = fi->shape[dim];
    nxo = xo->shape[0];
    nfo = nxo;
    nxi2 = nxi + 2;

    inxi  = (int) nxi;
    inxo  = (int) nxo;
    inxi2 = (int) nxi2;



/*
 * Some error checking. Make sure input dimension is valid.
 */
  if(dim < 0 || dim >= fi->ndim) {
    printf("linint1_n: Invalid dimension to do interpolation on, can't continue\n");
    return(1);
  }
  if(nxi < 2) {
/*
printf("dim: %d\n", dim);
printf("nxi: %d\n", nxi);
*/
    printf("linint1_n: xi must have at least 2 elements\n");
    return(2);
  }

/*
 * Test dimension sizes.
 */
  if((nxi > INT_MAX) || (nxo > INT_MAX) || (nxi2 > INT_MAX)) {
    printf("linint1_n: one or more dimension sizes is greater than INT_MAX\n");
    return(3);
  }

/*
 * Check dimensions of xi and fi. If xi is not one-dimensional, then it 
 * must be the same size as fi. Otherwise, the dims-th dimension of
 * fi must be equal to the length of xi.
 */
  if(xi->ndim > 1) {
    if(xi->ndim != fi->ndim) {
      printf("linint1_n: If xi is not one-dimensional, then it must be the same size as fi\n");
      return(4);
    }
    for(i = 0; i < fi->ndim; i++) {
      if(xi->shape[i] != fi->shape[i]) {
        printf("linint1_n: If xi is not one-dimensional, then it must be the same size as fi\n");
        return(5);
      }
    }
  }
  else {
    if(xi->shape[0] != nxi) {
      printf("linint1_n: The dim-th dimension of fi must be the same length as xi\n");
      return(6);
    }
  }


/*
 * Calculate size of leftmost dimensions (nl) up to the dim-th
 *   dimension.
 * Calculate size of rightmost dimensions (nr) from the
 *   dim-th dimension.
 *
 * The dimension to do the interpolation across is "dim".
 */
  nl = nr = 1;
  if(fi->ndim > 1) {
    nd = fi->ndim-1;
    for(i = 0; i < dim ; i++) {
      nl = nl*fi->shape[i];
    }
    for(i = dim+1; i < fi->ndim; i++) {
      nr = nr*fi->shape[i];
    }
  }
  else {
    nd = 1;
  }
  ntotal  = nr * nl;
  size_fo = ntotal * nfo;


/*
 * Coerce missing values.
 */
/*  coerce_missing(fi->type,has_missing_fi,&missing_fi,&missing_dfi, */
  coerce_missing(fi->type,has_missing_fi,&xmsg,&missing_dfi,
                 &missing_rfi);
/*
 * Allocate space for temporary output array.
  tmp_fo = (double*)calloc(nfo,sizeof(double));
  if(tmp_fo == NULL) {
    printf("linint1_n: Unable to allocate memory for temporary arrays\n");
    return(7);
  }
 */

/*
 * Allocate space for output array.
 */
/*  dsizes_fo = (size_t*)calloc(fi->ndim,sizeof(size_t));*/
  if(fi->type == NCOMP_DOUBLE) {
//    fo->addr   = (void*)calloc(size_fo,sizeof(double));
    fo->type   = NCOMP_DOUBLE;
/*    missing_fo = missing_dfi; */
  }
  else {
//    fo->addr   = (void*)calloc(size_fo,sizeof(float));
    fo->type   = NCOMP_FLOAT;
/*    missing_fo = missing_rfi; */
  }
  //if(fo == NULL || dsizes_fo == NULL) {
  if(fo->addr == NULL) {
    printf("linint1_n: Unable to allocate memory for output array\n");
    return(8);
  }
/* 
 * Go ahead and copy all dimesions, but then replace the dim-th one.
 */
/*
  for(i = 0; i < ndims_fi; i++) dsizes_fo[i] = dsizes_fi[i];
  dsizes_fo[*dim] = nxo;
*/

/*
 * Allocate space for work arrays.
  xiw  = (double*)calloc(nxi2,sizeof(double));
  fxiw = (double*)calloc(nxi2,sizeof(double));
  if(xiw == NULL || fxiw == NULL) {
    printf("linint1_n: Unable to allocate memory for work arrays\n");
    return(9);
  }
 */

/*
 * Coerce output array to double if necessary.
 */
  tmp_xo = coerce_input_double(xo->addr,xo->type,nxo,0,NULL,NULL);
  if(tmp_xo == NULL) {
    printf("linint1_n: Unable to coerce output array to double precision\n");
    return(10);
  }

/*
  if(xi->ndim == 1) {
    tmp_xi = coerce_input_double(xi->addr,xi->type,nxi,0,NULL,NULL);
  }
  else {
    tmp_xi = (double*)calloc(nxi,sizeof(double));
    if(tmp_xi == NULL) {
      printf("linint1_n: Unable to allocate memory for coercing input array to double precision\n");
      return(11);
    }
  }
*/

/*
  tmp_fi = (double*)calloc(nxi,sizeof(double));
  if(tmp_fi == NULL) {
    printf("linint1_n: Unable to allocate memory for coercing input array to double precision\n");
    return(12);
  }
*/


/*
 * Loop through leftmost and rightmost dimensions and call Fortran
 * routine for each array subsection.
 */
  nrnxi = nr*nxi;
  nrnxo = nr*nxo;
  for( i = 0; i < nl; i++ ) {
    index_nri = i*nrnxi;
    index_nro = i*nrnxo;
    for( j = 0; j < nr; j++ ) {
      index_fi = index_nri+j;
      index_fo = index_nro+j;

      if(xi->ndim > 1) {
        coerce_subset_input_double_step(xi->addr,tmp_xi,index_fi,nr,xi->type,
                                        nxi,0,NULL,NULL);
      }
      coerce_subset_input_double_step(fi->addr,tmp_fi,index_fi,nr,fi->type,
                                      nxi,0,NULL,NULL);










/*
 * Call Fortran routine.
 */
      dlinint1_(&inxi,tmp_xi,tmp_fi,&wrap,&inxo,tmp_xo,tmp_fo,
                xiw,fxiw,&inxi2,&missing_dfi,&opt,&ier);
/*
printf("tmp_fi[0]: %g\n", tmp_fi[0]);
printf("tmp_fo[0]: %g\n", tmp_fo[0]);
*/
/*                                 xiw,fxiw,&inxi2,&missing_dfi.doubleval, */

      if(ier) {
/*
printf("tmp_xi[0]: %g\n", tmp_xi[0]);
printf("tmp_xo[0]: %g\n", tmp_xo[0]);
printf("tmp_xi[1]: %g\n", tmp_xi[1]);
printf("tmp_xo[1]: %g\n", tmp_xo[1]);
*/
        printf("linint1_n: xi and xo must be monotonically increasing\n");
        return(7);

        set_subset_output_missing_step(fo->addr,index_fo,nr,fo->type,nfo,
/*                                       missing_dfi.doubleval); */
                                       missing_dfi);
      }
      else {
        coerce_output_float_or_double_step(fo->addr,tmp_fo,fi->type,nfo,index_fo,nr);
/*
printf("tmp_fo[0]: %g\n", tmp_fo[0]);
printf("fo[%d]: %g\n", index_fo, ((double*)fo->addr)[index_fo]);
*/
      }
    }
  }

}

int linint1_n_alloc_work(ncomp_array* xi, ncomp_array* fi, ncomp_array* xo, ncomp_array* fo, int wrap, double xmsg, int opt, int dim)
{
    double *tmp_xi, *tmp_fi, *tmp_fo, *xiw, *fxiw;

    size_t nxi, nxo, nfo, nxi2;
    nxi = fi->shape[dim];
    nxo = xo->shape[0];
    nfo = nxo;
    nxi2 = nxi + 2;

    if(xi->ndim == 1) {
        tmp_xi = coerce_input_double(xi->addr,xi->type,nxi,0,NULL,NULL);
    }
    else {
        tmp_xi = (double*)calloc(nxi,sizeof(double));
        if(tmp_xi == NULL) {
            printf("linint1_n: Unable to allocate memory for coercing input array to double precision\n");
            return(11);
        }
    }

    tmp_fi = (double*)calloc(nxi,sizeof(double));
    if(tmp_fi == NULL) {
        printf("linint1_n: Unable to allocate memory for coercing input array to double precision\n");
        return(12);
    }

/*
 * Allocate space for temporary output array.
 */
  tmp_fo = (double*)calloc(nfo,sizeof(double));
  if(tmp_fo == NULL) {
    printf("linint1_n: Unable to allocate memory for temporary arrays\n");
    return(7);
  }

/*
 * Allocate space for work arrays.
 */
  xiw  = (double*)calloc(nxi2,sizeof(double));
  fxiw = (double*)calloc(nxi2,sizeof(double));
  if(xiw == NULL || fxiw == NULL) {
    printf("linint1_n: Unable to allocate memory for work arrays\n");
    return(9);
  }

    int ier = linint1_n_loop(xi, fi, xo, fo, tmp_xi, tmp_fi, tmp_fo, xiw, fxiw, wrap, xmsg, opt, dim);

    if(xi->ndim > 1 || xi->type != NCOMP_DOUBLE) free(tmp_xi);
    free(tmp_fi);
    free(tmp_fo);
    free(xiw);
    free(fxiw);
    return ier;
}

int linint1_n(ncomp_array* xi, ncomp_array* fi, ncomp_array* xo, ncomp_array* fo, int wrap, double xmsg, int opt, int dim)
{
    return linint1_n_alloc_work(xi, fi, xo, fo, wrap, xmsg, opt, dim);
}
