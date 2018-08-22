extern void vinth2p_(double *, double *, double *, double *,
                     double *, double *, double *,int *,
                     int *, double *, double *, int *,
                     int *, int *, int *, int *, int *);

void vinth2p(double *datai, double *datao, double *hbcofa, double *hbcofb,
             double *p0, double *plevi, double *plevo, int *intyp, int *ilev,
             double *psfc, double *msg, int *kxtrp, int *nlevi, int *nlat,
             int *nlon, int *nlevo)
{
    int nlevip1 = *nlevi + 1;
    return vinth2p_(datai, datao, hbcofa, hbcofb,
                    p0, plevi, plevo, intyp, ilev, 
                    psfc, msg, kxtrp, nlon, nlat,
                    nlevi, &nlevip1, nlevo);
}
