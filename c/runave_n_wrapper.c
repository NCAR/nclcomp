#include <stdlib.h>
#include <limits.h>

extern void drunave_(double*,int*,int*,int*,double*,double*,
                     int*,int*);

int ncl_drunave_n_wrapper(double *x, long npts, long nave, int kopt, double xmsg)
{

    double *work;
    long lwork = npts + 2*(nave/2);
    int ier, inpts, inave, ilwork;

    work = (double *)calloc(lwork,sizeof(double));

    if((npts > INT_MAX) || (lwork > INT_MAX) || (nave > INT_MAX)) return -100;
    inpts = (int)npts;
    inave = (int)nave;
    ilwork = (int)lwork;

    drunave_(x, &inpts, &inave, &kopt, &xmsg, work, &ilwork, &ier);
    return ier;
}
