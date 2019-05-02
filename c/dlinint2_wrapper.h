int linint2_old(
    void*, int, size_t*, int, void*, int, size_t*, int,
    void*, int, size_t*, int, void*, int, size_t*, int,
    void*, int, size_t*, int, void*, int, size_t*, int,
    int, double, int);

int linint2_loop(
    void*, int, size_t*, int, void*, int, size_t*, int,
    void*, int, size_t*, int, void*, int, size_t*, int,
    void*, int, size_t*, int, void*, int, size_t*, int,
    double*, double*, double*, int, double, int);


int linint2_struct(ncomp_array_old, ncomp_array_old, ncomp_array_old,
                   ncomp_array_old, ncomp_array_old, ncomp_array_old*,
                   int, double, int);

int linint2_struct_new(ncomp_array*, ncomp_array*, ncomp_array*,
                       ncomp_array*, ncomp_array*, ncomp_array*,
                       int, double, int);

int linint2(const ncomp_array*, const ncomp_array*, const ncomp_array*,
            const ncomp_array*, const ncomp_array*, ncomp_array*,
            int, double, int);

int linint2_alloc_output(const ncomp_array*, const ncomp_array*, const ncomp_array*,
                         const ncomp_array*, const ncomp_array*, ncomp_array**,
                         int, double, int);
