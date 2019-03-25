#include <stdlib.h>
#include "ncomp.h"
#include "util.h"

/*void _to_double(void* arr, int ndims, long* dim_sizes, int arr_type, double* out_arr) {*/
void _to_double(void* arr, size_t arr_index, size_t arr_size, int arr_type, double* out_arr) {
    long i;
/*
    long total_size = 1;
    for (i = 0; i < ndims; i++)
        total_size *= dim_sizes[i];
*/

    switch(arr_type) {
        case NCOMP_DOUBLE:
            /*
            out_arr = &((double*)arr)[arr_index];
            */
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((double*)arr)[i + arr_index];
            break;
        case NCOMP_FLOAT:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((float*)arr)[i + arr_index];
            break;
        case NCOMP_BOOL:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((char*)arr)[i + arr_index];
            break;
        case NCOMP_BYTE:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((signed char*)arr)[i + arr_index];
            break;
        case NCOMP_UBYTE:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((unsigned char*)arr)[i + arr_index];
            break;
        case NCOMP_SHORT:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((short*)arr)[i + arr_index];
            break;
        case NCOMP_USHORT:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((unsigned short*)arr)[i + arr_index];
            break;
        case NCOMP_INT:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((int*)arr)[i + arr_index];
            break;
        case NCOMP_UINT:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((unsigned int*)arr)[i + arr_index];
            break;
        case NCOMP_LONG:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((long*)arr)[i + arr_index];
            break;
        case NCOMP_ULONG:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((unsigned long*)arr)[i + arr_index];
            break;
        case NCOMP_LONGLONG:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((long long*)arr)[i + arr_index];
            break;
        case NCOMP_ULONGLONG:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((unsigned long long*)arr)[i + arr_index];
            break;
        case NCOMP_LONGDOUBLE:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (double)((long double*)arr)[i + arr_index];
            break;
    }

    return;
}

/*void _to_float(void* arr, int ndims, long* dim_sizes, int arr_type, float* out_arr) {*/
void _to_float(void* arr, size_t arr_index, size_t arr_size, int arr_type, float* out_arr) {
    long i;
/*
    long total_size = 1;
    for (i = 0; i < ndims; i++)
        total_size *= dim_sizes[i];

    float* out_arr = (float*)calloc(total_size, sizeof(float));
*/

    switch(arr_type) {
        case NCOMP_FLOAT:
            /*
            out_arr = &((float*)arr)[arr_index];
            */
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((float*)arr)[i + arr_index];
            break;
        case NCOMP_DOUBLE:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((double*)arr)[i + arr_index];
            break;
        case NCOMP_BOOL:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((char*)arr)[i + arr_index];
            break;
        case NCOMP_BYTE:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((signed char*)arr)[i + arr_index];
            break;
        case NCOMP_UBYTE:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((unsigned char*)arr)[i + arr_index];
            break;
        case NCOMP_SHORT:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((short*)arr)[i + arr_index];
            break;
        case NCOMP_USHORT:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((unsigned short*)arr)[i + arr_index];
            break;
        case NCOMP_INT:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((int*)arr)[i + arr_index];
            break;
        case NCOMP_UINT:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((unsigned int*)arr)[i + arr_index];
            break;
        case NCOMP_LONG:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((long*)arr)[i + arr_index];
            break;
        case NCOMP_ULONG:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((unsigned long*)arr)[i + arr_index];
            break;
        case NCOMP_LONGLONG:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((long long*)arr)[i + arr_index];
            break;
        case NCOMP_ULONGLONG:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((unsigned long long*)arr)[i + arr_index];
            break;
        case NCOMP_LONGDOUBLE:
            for (i = 0; i < arr_size; i++)
                out_arr[i] = (float)((long double*)arr)[i + arr_index];
            break;
    }

    return;
}


void coerce_missing(
int type_x,
int has_missing_x,
void* missing_x,
double* missing_dx,
float* missing_rx)
{
/*
 * Check for missing value and coerce if neccesary.
 */
  if(has_missing_x) {
/*
 * Coerce missing value to double.
 */ 
    _to_double(missing_x, 0, 1, type_x, missing_dx);
    
    if(type_x != NCOMP_DOUBLE && missing_rx != NULL) {
        _to_float(missing_x, 0, 1, type_x, missing_rx);
    }
  }
  else {
    if(missing_dx != NULL) {
/*
 * Get the default missing value, just in case.
 */   
      if(type_x != NCOMP_DOUBLE) {
        _to_double(missing_x, 0, 1, type_x, missing_dx);
        if(missing_rx != NULL) { 
          _to_float(missing_x, 0, 1, type_x, missing_rx);
        }
      }
      else {
        _to_double(missing_x, 0, 1, type_x, missing_dx);
      }
    }
  }
}


/*ncomp_array* create_ncomp_array(void* array_ptr, int array_type, int array_ndim, size_t* array_shape) {*/
ncomp_array* ncomp_array_alloc(void* array_ptr, int array_type, int array_ndim, size_t* array_shape) {
    ncomp_array* new_array = (ncomp_array*)malloc(sizeof(ncomp_array) + (array_ndim - 1) * sizeof(size_t));
    new_array->type = array_type;
    new_array->addr = array_ptr;
    new_array->ndim = array_ndim;
    int i;
    for (i = 0; i < array_ndim; i++)
        new_array->shape[i] = array_shape[i];
    return new_array;
}

void ncomp_array_free(ncomp_array* old_array, int keep_data_ptr) {
    /* free ptr by default */
    if (!keep_data_ptr)
        free(old_array->addr);

    free(old_array);

    return;
}
