#include <stdio.h>
#include <stdlib.h>
extern void dwgtareaave_(double *t, double *wgty, double *wgtx, int *mx, int *ny, double *tmsg, int *iflag, double *ave);

void ncl_wgt_areaave(double *t, double *wgtx, double *wgty, int mx, int ny, double tmsg, int iflag, double *ave)
{
    dwgtareaave_(t, wgty, wgtx, &mx, &ny, &tmsg, &iflag, ave);
    return;
}
