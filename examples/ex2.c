#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cnpy.h"

int main(void) {
  char *fn = "data_double.npy";
  size_t dims[] = {2, 101};
  size_t n_dim = sizeof(dims) / sizeof(dims[0]);
  cnpy_array a;
  cnpy_byte_order byte_order = CNPY_BE; /* big endian */
  cnpy_dtype dtype = CNPY_F8; /* double */
  cnpy_flat_order order = CNPY_FORTRAN_ORDER; /* Fortran (column major) order */
  if (cnpy_create(fn, byte_order, dtype, order, n_dim, dims, &a) != CNPY_SUCCESS) {
    cnpy_perror("Unable to create file 'data_double.npy'");
    abort();
  }
  size_t index[] = {0, 0};
  for (size_t i = 0; i < dims[1]; i += 1) {
    double t = 2 * M_PI * i / (double) (dims[1] - 1);
    index[0] = 0;
    index[1] = i;
    cnpy_set_f8(a, index, t);
    index[0] = 1;
    index[1] = i;
    cnpy_set_f8(a, index, sin(t));
  }
  /* optional: */
  if (cnpy_close(&a) != CNPY_SUCCESS) {
    cnpy_perror("Unable to close file 'data_double.npy'");
    abort();
  }
  return EXIT_SUCCESS;
}
