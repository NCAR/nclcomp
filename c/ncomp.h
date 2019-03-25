#include <stddef.h>
#include "vinth_wrapper.h"
#include "wgt_areaave_wrapper.h"
#include "dtrend_msg_n_wrapper.h"
#include "runave_n_wrapper.h"
#include "constants.h"

#ifndef NCLCOMP_H
#define NCLCOMP_H
/* enumerated types, adapted from numpy's ndarraytypes.h */
enum NCOMP_TYPES {NCOMP_BOOL=0,
                  NCOMP_BYTE, NCOMP_UBYTE,
                  NCOMP_SHORT, NCOMP_USHORT,
                  NCOMP_INT, NCOMP_UINT,
                  NCOMP_LONG, NCOMP_ULONG,
                  NCOMP_LONGLONG, NCOMP_ULONGLONG,
                  NCOMP_FLOAT, NCOMP_DOUBLE, NCOMP_LONGDOUBLE
};


typedef struct {
    /* ordered for efficient memory packing */
    int     type;
    int     ndim;
    void*   addr;
    size_t* shape;
} ncomp_array;

typedef struct {
    /* ordered for efficient memory packing */
    int     type;
    int     ndim;
    void*   addr;
    size_t  shape[1]; /* use ndim to malloc extra space for dims */
} ncomp_array_shape;
#endif
