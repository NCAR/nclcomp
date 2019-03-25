#include <stdlib.h>
#include <stdio.h>
#include "ncomp.h"
#include "util.h"

extern void dlinint2_(int *,double *,int *,double *,
                      double *,int *,int *,double *,int *,
                      double *,double *,double *, double *,
                      int *,double *,int *,int *);

/*
int dlinint2(int inxi, double* xi, int inyi, double* yi, double* fi, int icycx, int inxo, double* xo, int inyo, double* yo, double* fo, double* xiw, double* fxiw, int nxi2, double xmsg, int iopt) {
    int ier;
    dlinint2_(&inxi, xi, &inyi, yi, fi, &icycx, &inxo, xo, &inyo, yo, fo, xiw, fxiw, &nxi2, &xmsg, &iopt, &ier);
    return ier;
}
*/


int dlinint2_c2f(int inxi, double* xi, int inyi, double* yi, double* fi, int icycx, int inxo, double* xo, int inyo, double* yo, double* fo, double* xiw, double* fxiw, int nxi2, double xmsg, int iopt) {
/*
    int nxi2 = nxi + 2;
    double* xiw  = (double*)calloc(nxi2,sizeof(double));
    double* fxiw = (double*)calloc(nxi2,sizeof(double));
*/
    int ier;

    dlinint2_(&inxi, xi, &inyi, yi, fi, &icycx, &inxo, xo, &inyo, yo, fo, xiw, fxiw, &nxi2, &xmsg, &iopt, &ier);

    free(xiw);
    free(fxiw);
    return ier;
}

/*int dlinint2_loop(*/
int linint2(
    void* xi, int ndims_xi, size_t* dsizes_xi, int type_xi,
    void* yi, int ndims_yi, size_t* dsizes_yi, int type_yi,
    void* fi, int ndims_fi, size_t* dsizes_fi, int type_fi,
    void* xo, int ndims_xo, size_t* dsizes_xo, int type_xo,
    void* yo, int ndims_yo, size_t* dsizes_yo, int type_yo,
    void* fo, int ndims_fo, size_t* dsizes_fo, int type_fo,
    int icycx, double xmsg, int iopt
) {
    double* xiw  = (double*)calloc(dsizes_xi[ndims_xi-1] + 2,sizeof(double));
    double* fxiw = (double*)calloc(dsizes_xi[ndims_xi-1] + 2,sizeof(double));
    double* tmp_fo = (double*)calloc(dsizes_xo[0] * dsizes_yo[0], sizeof(double));
    int ier = linint2_loop(xi, ndims_xi, dsizes_xi, type_xi,
                           yi, ndims_yi, dsizes_yi, type_yi,
                           fi, ndims_fi, dsizes_fi, type_fi,
                           xo, ndims_xo, dsizes_xo, type_xo,
                           yo, ndims_yo, dsizes_yo, type_yo,
                           fo, ndims_fo, dsizes_fo, type_fo,
                           xiw, fxiw, tmp_fo, icycx, xmsg, iopt);
    free(xiw);
    free(fxiw);
    free(tmp_fo);
    return ier;
}

int linint2_loop(
    void* xi, int ndims_xi, size_t* dsizes_xi, int type_xi,
    void* yi, int ndims_yi, size_t* dsizes_yi, int type_yi,
    void* fi, int ndims_fi, size_t* dsizes_fi, int type_fi,
    void* xo, int ndims_xo, size_t* dsizes_xo, int type_xo,
    void* yo, int ndims_yo, size_t* dsizes_yo, int type_yo,
    void* fo, int ndims_fo, size_t* dsizes_fo, int type_fo,
    double* xiw, double* fxiw, double* tmp_fo,
    int icycx, double xmsg, int iopt
) {
/*
    printf("xi: %p\n", xi);
    printf("yi: %p\n", yi);
    printf("fi: %p\n", fi);
    printf("xo: %p\n", xo);
    printf("yo: %p\n", yo);
    printf("fo: %p\n", fo);
*/
    /* FORTRAN ERROR CODES
    ier     - error code
        =0;   no error
        =1;   not enough points in input/output array
        =2;   xi or xo are not monotonically (in/de)creasing
        =3;   yi or yo are not monotonically (in/de)creasing
    */

    long nxi, nyi, nxi2, nfi, nxo, nyo, nfo, size_leftmost, size_fo;

    /*double* fo;*/

    nxi  = dsizes_xi[ndims_xi-1];
    nyi  = dsizes_yi[ndims_yi-1];
    nxo  = dsizes_xo[0];
    nyo  = dsizes_yo[0];
    nxi2 = nxi + 2;
    nfi  = nxi * nyi;
    nfo  = nxo * nyo;

    int inxi = (int) nxi;
    int inyi = (int) nyi;
    int inxo = (int) nxo;
    int inyo = (int) nyo;
    int inxi2 = (int) nxi2;
    int infi = (int) nfi;
    int info = (int) nfo;


    /* subset input arrays */
    double *tmp_xi = NULL;
    double *tmp_yi = NULL;
    double *tmp_fi = NULL;
    double *tmp_xo, *tmp_yo;
/*
    double* xiw  = (double*)calloc(nxi2,sizeof(double));
    double* fxiw = (double*)calloc(nxi2,sizeof(double));
*/
    int ier = 0;
    int tmp_ier;

    long i, j, index_xi = 0, index_yi = 0, index_fi = 0, index_fo = 0;

    /*
    tmp_fo = (double*)calloc(nfo, sizeof(double));
    */
    tmp_xo = (double*)xo;
    tmp_yo = (double*)yo;

  size_leftmost = 1;
  for( i = 0; i < ndims_fi-2; i++ ) size_leftmost *= dsizes_fi[i];
  size_fo = size_leftmost * nfo;

    for( i = 0; i < size_leftmost; i++ ) {
        if(ndims_xi > 1 || i == 0) {
            if(type_xi != NCOMP_DOUBLE) { 
                _to_double(xi, index_xi, nxi, type_xi, tmp_xi);
            /*
                coerce_subset_input_double(xi,tmp_xi,index_xi,type_xi,nxi,0,NULL,NULL);
            */
            }
            else {
                tmp_xi = &((double*)xi)[index_xi];
                /*
                printf("tmp_xi: %p\n", tmp_xi);
                */
            }
            tmp_xi = &((double*)xi)[index_xi];
        }
        if(ndims_yi > 1 || i == 0) {
            if(type_yi != NCOMP_DOUBLE) { 
                _to_double(yi, index_yi, nyi, type_yi, tmp_yi);
            /*
                coerce_subset_input_double(yi,tmp_yi,index_yi,type_yi,nyi,0,NULL,NULL);
            */
            }
            else {
                tmp_yi = &((double*)yi)[index_yi];
            }
            tmp_yi = &((double*)yi)[index_yi];
        }

        if(type_fi != NCOMP_DOUBLE) { 
            _to_double(fi, index_fi, nfi, type_fi, tmp_fi);
        /*
            coerce_subset_input_double(fi,tmp_fi,index_fi,type_fi,nfi,0,NULL,NULL);
        */
        }
        else {
            tmp_fi = &((double*)fi)[index_fi];
            /*
            printf("fi: %p\n", fi);
            printf("tmp_fi: %p\n", tmp_fi);
            */
        }

        tmp_fi = &((double*)fi)[index_fi];

        tmp_ier = 0;
        // tmp_ier = dlinint2_c2f(inxi, tmp_xi, inyi, tmp_yi, tmp_fi, icycx, inxo, xo, inyo, yo, fo, xmsg, iopt);
        /*
        printf("fo: %p\n", fo);
        printf("tmp_fo: %p\n", tmp_fo);
        */
/*
printf("inxi:   %d\n", inxi);
printf("inyi:   %d\n", inyi);
printf("inxo:   %d\n", inxo);
printf("inyo:   %d\n", inyo);
printf("inxi2:  %d\n", inxi2);
*/
        dlinint2_(&inxi, tmp_xi, &inyi, tmp_yi, tmp_fi, &icycx, &inxo, tmp_xo, &inyo, tmp_yo, tmp_fo, xiw, fxiw, &inxi2, &xmsg, &iopt, &tmp_ier);
        /*
        for(j = 0; j < nfi; j++)
            printf("tmp_fi[%d]: %g\n", j, tmp_fi[j]);
        for(j = 0; j < nfo; j++)
            printf("tmp_fo[%d]: %g\n", j, tmp_fo[j]);
        */

        ier |= tmp_ier;
        /*
        printf("ier: %d\n", ier);
        */
        if(tmp_ier) {
            /*
            NhlPError(NhlWARNING,NhlEUNKNOWN,"linint2: xi, yi, xo, and yo must be monotonically increasing");
            */
            for(j = 0; j < nfo; j++) {
                if(type_fi == NCOMP_DOUBLE) { 
                    ((double*)fo)[index_fo+j] = xmsg;
                }
                else {
                    /* FIX THIS ********/
                    ((float*)fo)[index_fo+j] = (float)xmsg;
                }
            }
        }
        else {
            /*
            coerce_output_float_or_double(fo,tmp_fo,type_fi,nfo,index_fo);
            _to_double(xi, index_xi, nxi, type_xi, tmp_xi);
            */
            _to_double(tmp_fo, 0, nfo, type_fo, ((double*)fo) + index_fo);
            /*
            printf("tmp_fo[0]: %g\n", tmp_fo[0]);
            printf("fo[%d]: %g\n", index_fo, ((double*)fo)[index_fo]);
            */
        }
        if(ndims_xi > 1) index_xi += nxi;
        if(ndims_yi > 1) index_yi += nyi;
        index_fi += nfi;
        index_fo += nfo;
    }



/*
    free(xiw);
    free(fxiw);
*/
    return ier;

}

/*
int dlinint2() {
  nxi  = dsizes_xi[ndims_xi-1];
  nyi  = dsizes_yi[ndims_yi-1];
  nxo  = dsizes_xo[0];
  nyo  = dsizes_yo[0];


}
*/
int linint2_struct(ncomp_array xi, ncomp_array yi, ncomp_array fi,
                   ncomp_array xo, ncomp_array yo, ncomp_array* fo,
                   int icycx, double xmsg, int iopt) {
    return linint2(xi.addr, xi.ndim, xi.shape, xi.type,
                   yi.addr, yi.ndim, yi.shape, yi.type,
                   fi.addr, fi.ndim, fi.shape, fi.type,
                   xo.addr, xo.ndim, xo.shape, xo.type,
                   yo.addr, yo.ndim, yo.shape, yo.type,
                   fo->addr, fo->ndim, fo->shape, fo->type,
                   icycx, xmsg, iopt);
}

int linint2_alloc_work(
    void* xi, int ndims_xi, size_t* dsizes_xi, int type_xi,
    void* yi, int ndims_yi, size_t* dsizes_yi, int type_yi,
    void* fi, int ndims_fi, size_t* dsizes_fi, int type_fi,
    void* xo, int ndims_xo, size_t* dsizes_xo, int type_xo,
    void* yo, int ndims_yo, size_t* dsizes_yo, int type_yo,
    void* fo, int ndims_fo, size_t* dsizes_fo, int type_fo,
    int icycx, double xmsg, int iopt
) {
    double* xiw  = (double*)calloc(dsizes_xi[ndims_xi-1] + 2,sizeof(double));
    double* fxiw = (double*)calloc(dsizes_xi[ndims_xi-1] + 2,sizeof(double));
    double* tmp_fo = (double*)calloc(dsizes_xo[0] * dsizes_yo[0], sizeof(double));
    int ier = linint2_loop(xi, ndims_xi, dsizes_xi, type_xi,
                           yi, ndims_yi, dsizes_yi, type_yi,
                           fi, ndims_fi, dsizes_fi, type_fi,
                           xo, ndims_xo, dsizes_xo, type_xo,
                           yo, ndims_yo, dsizes_yo, type_yo,
                           fo, ndims_fo, dsizes_fo, type_fo,
                           xiw, fxiw, tmp_fo, icycx, xmsg, iopt);
    free(xiw);
    free(fxiw);
    free(tmp_fo);
    return ier;
}


int linint2_alloc_output(
    void* xi, int ndims_xi, size_t* dsizes_xi, int type_xi,
    void* yi, int ndims_yi, size_t* dsizes_yi, int type_yi,
    void* fi, int ndims_fi, size_t* dsizes_fi, int type_fi,
    void* xo, int ndims_xo, size_t* dsizes_xo, int type_xo,
    void* yo, int ndims_yo, size_t* dsizes_yo, int type_yo,
    void** fo, int* ndims_fo, size_t** dsizes_fo, int* type_fo,
    int icycx, double xmsg, int iopt
) {
    int i;
    long nfo;
    ndims_fo = (int*)calloc(1, sizeof(int));
    *ndims_fo = ndims_fi;
    type_fo = (int*)calloc(1, sizeof(int));
    *type_fo = type_fi;

    *dsizes_fo = (size_t*)calloc(ndims_fi, sizeof(size_t));
    for (i = 0; i < *ndims_fo - 2; i++)
        (*dsizes_fo)[i] = dsizes_fi[i];
    (*dsizes_fo)[*ndims_fo-2] = dsizes_yo[0];
    (*dsizes_fo)[*ndims_fo-1] = dsizes_xo[0];

    nfo = 1;
    for (i = 0; i < *ndims_fo; i++)
        nfo *= (*dsizes_fo)[i];

    *fo = (double*)calloc(nfo, sizeof(double));

    int ier = linint2_alloc_work(xi, ndims_xi, dsizes_xi, type_xi,
                           yi, ndims_yi, dsizes_yi, type_yi,
                           fi, ndims_fi, dsizes_fi, type_fi,
                           xo, ndims_xo, dsizes_xo, type_xo,
                           yo, ndims_yo, dsizes_yo, type_yo,
                           *fo, *ndims_fo, *dsizes_fo, *type_fo,
                           icycx, xmsg, iopt);
    /* free fo, ndims_fo, dsizes_fo, type_fo */
    return ier;
}

int linint2_public(ncomp_array xi, ncomp_array yi, ncomp_array fi,
                   ncomp_array xo, ncomp_array yo, ncomp_array* fo,
                   int icycx, double xmsg, int iopt) {
    return linint2(xi.addr, xi.ndim, xi.shape, xi.type,
                   yi.addr, yi.ndim, yi.shape, yi.type,
                   fi.addr, fi.ndim, fi.shape, fi.type,
                   xo.addr, xo.ndim, xo.shape, xo.type,
                   yo.addr, yo.ndim, yo.shape, yo.type,
                   fo->addr, fo->ndim, fo->shape, fo->type,
                   icycx, xmsg, iopt);
}
