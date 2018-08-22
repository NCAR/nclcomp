#include <stdio.h>
#include <stdlib.h>

#include "c/vinth_wrapper.h"

int main(void) {
    double *datai;
    double *datao;
    double *hbcofa;
    double *hbcofb;
    double *p0;
    double *plevi;
    double *plevo;
    int *intyp;
    int *ilev;
    double *psfc;
    double *msg;
    int *kxtrp;
    int *nlevi;
    int *nlat;
    int *nlon;
    int *nlevo;
    vinth_cw(datai, datao, hbcofa, hbcofb, p0, plevi, plevo, intyp, ilev, psfc, msg, kxtrp, nlon, nlat, nlevi, nlevo);
}
