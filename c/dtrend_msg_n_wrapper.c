void ddtrndmsg_(const double *x, const double *y, const int *npts,
                const double *xmsg, const double *ymsg, const int *iopt,
                const double *ydt, double *slope, double *yint, const int *ier);

void ncl_dtrend_msg_n(const double *x, const double *y, const int npts,
                      const double xmsg, const double ymsg, const int iopt,
                      const double *ydt, double *slope, double *yint,
                      int *ier)
{
    ddtrndmsg_(x, y, &npts, &xmsg, &ymsg, &iopt, ydt, slope, yint, ier);
    return;
}
