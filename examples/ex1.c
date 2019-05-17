#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <complex.h>
#include <assert.h>
#include "cnpy.h"

int main(void) {
  char *fn = "data_complex_2d.npy";
  cnpy_array a;
  bool writable = false;
  if (cnpy_open(fn, writable, &a) != CNPY_SUCCESS) {
    cnpy_perror("Unable to load file 'data_complex_2d.npy'");
    abort();
  }
  assert(a.dtype == CNPY_C16); /* C16 == complex double */
  assert(a.n_dim == 2);
  size_t index[] = {0, 0};
  for (index[0] = 0; index[0] < a.dims[0]; index[0] += 1) {
    for (index[1] = 0; index[1] < a.dims[1]; index[1] += 1) {
      complex double x = cnpy_get_c16(a, index);
      printf("%.5lf + %.5lf * I  ", creal(x), cimag(x));
      /* The following works no matter if writable = true or writable = false.
       * If writable == true, the loaded file is changed accordingly.
       * If writable == false, the changes are only in memory and do not change the file. */
      cnpy_set_c16(a, index, index[0]+index[1]*I);
    }
    printf("\n");
  }
  /* optional: */
  if (cnpy_close(&a) != CNPY_SUCCESS) {
    cnpy_perror("Unable to close file 'data_complex_2d.npy'");
    abort();
  }
  return EXIT_SUCCESS;
}
