#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <complex.h>
#include <inttypes.h>
#include <errno.h>
#include "cnpy.h"

_Static_assert(sizeof(complex float) == 8, "");
_Static_assert(sizeof(complex double) == 16, "");
#ifndef MAP_ANONYMOUS
#error "anonymous mmap is required"
#endif

bool equal_f(float x1, float x2) {
  /* This is needed because of NaN */
  return memcmp((char *) &x1, (char *) &x2, sizeof(float)) == 0;
}

bool equal_d(double x1, double x2) {
  /* This is needed because of NaN */
  return memcmp((char *) &x1, (char *) &x2, sizeof(double)) == 0;
}

bool equal_cf(complex float x1, complex float x2) {
  return memcmp((char *) &x1, (char *) &x2, sizeof(complex float)) == 0;
}

bool equal_cd(complex double x1, complex double x2) {
  return memcmp((char *) &x1, (char *) &x2, sizeof(complex double)) == 0;
}

void print_index(size_t n_dim, const size_t * const index) {
  printf("(");
  for (size_t i = 0; i < n_dim; i += 1) {
    printf("%zu", index[i]);
    if (i + 1 < n_dim) {
      printf(", ");
    }
  }
  printf(")");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    assert(argc > 0);
    fprintf(stderr, "Usage: %s <filename>", argv[0]);
    abort();
  }

  char *fn = argv[1];

  /* Open input array */
  cnpy_array a_in;
  if (cnpy_open(fn, true, &a_in) != CNPY_SUCCESS) {
    cnpy_perror("Input file not opened");
    return EXIT_SUCCESS;
  }

  /* Print header information */
  printf("dtype: ");
  switch(a_in.dtype) {
    case CNPY_B:
      printf("bool\n");
      break;
    case CNPY_I1:
      printf("1 byte signed int\n");
      break;
    case CNPY_I2:
      printf("2 byte signed int\n");
      break;
    case CNPY_I4:
      printf("4 byte signed int\n");
      break;
    case CNPY_I8:
      printf("8 byte signed int\n");
      break;
    case CNPY_U1:
      printf("1 byte unsigned int\n");
      break;
    case CNPY_U2:
      printf("2 byte unsigned int\n");
      break;
    case CNPY_U4:
      printf("4 byte unsigned int\n");
      break;
    case CNPY_U8:
      printf("8 byte unsigned int\n");
      break;
    case CNPY_F4:
      printf("4 byte single-precision float\n");
      break;
    case CNPY_F8:
      printf("8 byte double-precision float (aka double)\n");
      break;
    case CNPY_C8:
      printf("8 byte complex (float, float)\n");
      break;
    case CNPY_C16:
      printf("16 byte complex (double, double)\n");
      break;
    default:
      assert(false);
  }
  switch(a_in.byte_order) {
    case CNPY_BE:
      printf("byte order: big-endian\n");
      break;
    case CNPY_LE:
      printf("byte order: little-endian\n");
      break;
    case CNPY_NE:
      printf("byte order: none (single-byte dtype)\n");
      break;
    default:
      assert(false);
  }
  printf("shape: ");
  print_index(a_in.n_dim, a_in.dims);
  printf("\n");
  switch (a_in.order) {
    case CNPY_FORTRAN_ORDER:
      printf("Fortran (column-major) order\n\n");
      break;
    case CNPY_C_ORDER:
      printf("C (row-major) order\n\n");
      break;
  }

  /* Open copy (in memory only) */
  cnpy_array a_out1;
  if (cnpy_create(NULL, a_in.byte_order, a_in.dtype, a_in.order, a_in.n_dim, a_in.dims, &a_out1) != CNPY_SUCCESS) {
    cnpy_perror("Copy file not opened");
    abort();
  }
  cnpy_array a_out2;
  if (cnpy_create(NULL, (a_in.byte_order == CNPY_LE)? CNPY_BE : CNPY_LE, a_in.dtype, (a_in.order == CNPY_FORTRAN_ORDER)? CNPY_C_ORDER : CNPY_FORTRAN_ORDER, a_in.n_dim, a_in.dims, &a_out2) != CNPY_SUCCESS) {
    cnpy_perror("Copy file not opened");
    abort();
  }

  /* Now copy entries of a_in to a_out1 and a_out2.
   * Also compare the value of a_out1 and a_out2 afterwards.
   * Also print the results. */
  size_t index[CNPY_MAX_DIM];
  for (size_t i = 0; i < a_in.n_dim; i += 1) {
    index[i] = 0;
  }
  size_t i = 0;

  do {
    print_index(a_in.n_dim, index);
    printf(", %zu: ", i);
    assert (i == cnpy_flatten_index(a_in, index));
    i += 1;

    switch (a_in.dtype) {
      case CNPY_B: {
        bool value = cnpy_get_b(a_in, index);
        printf("%s", value? "true" : "false");
        fflush(stdout);
        cnpy_set_b(a_out1, index, value);
        cnpy_set_b(a_out2, index, value);
        assert(cnpy_get_b(a_out1, index) == value);
        assert(cnpy_get_b(a_out1, index) == cnpy_get_b(a_out2, index));
        }
        break;
      case CNPY_I1: {
        int8_t value = cnpy_get_i1(a_in, index);
        printf("%"PRIi8, value);
        fflush(stdout);
        cnpy_set_i1(a_out1, index, value);
        cnpy_set_i1(a_out2, index, value);
        assert(cnpy_get_i1(a_out1, index) == value);
        assert(cnpy_get_i1(a_out1, index) == cnpy_get_i1(a_out2, index));
        }
        break;
      case CNPY_I2: {
        int16_t value = cnpy_get_i2(a_in, index);
        printf("%"PRIi16, value);
        fflush(stdout);
        cnpy_set_i2(a_out1, index, value);
        cnpy_set_i2(a_out2, index, value);
        assert(cnpy_get_i2(a_out1, index) == value);
        assert(cnpy_get_i2(a_out1, index) == cnpy_get_i2(a_out2, index));
        }
        break;
      case CNPY_I4: {
        int32_t value = cnpy_get_i4(a_in, index);
        printf("%"PRIi32, value);
        fflush(stdout);
        cnpy_set_i4(a_out1, index, value);
        cnpy_set_i4(a_out2, index, value);
        assert(cnpy_get_i4(a_out1, index) == value);
        assert(cnpy_get_i4(a_out1, index) == cnpy_get_i4(a_out2, index));
        }
        break;
      case CNPY_I8: {
        int64_t value = cnpy_get_i8(a_in, index);
        printf("%"PRIi64, value);
        fflush(stdout);
        cnpy_set_i8(a_out1, index, value);
        cnpy_set_i8(a_out2, index, value);
        assert(cnpy_get_i8(a_out1, index) == value);
        assert(cnpy_get_i8(a_out1, index) == cnpy_get_i8(a_out2, index));
        }
        break;
      case CNPY_U1: {
        uint8_t value = cnpy_get_u1(a_in, index);
        printf("%"PRIu8, value);
        fflush(stdout);
        cnpy_set_u1(a_out1, index, value);
        cnpy_set_u1(a_out2, index, value);
        assert(cnpy_get_u1(a_out1, index) == value);
        assert(cnpy_get_u1(a_out1, index) == cnpy_get_u1(a_out2, index));
        }
        break;
      case CNPY_U2: {
        uint16_t value = cnpy_get_u2(a_in, index);
        printf("%"PRIu16, value);
        fflush(stdout);
        cnpy_set_u2(a_out1, index, value);
        cnpy_set_u2(a_out2, index, value);
        assert(cnpy_get_u2(a_out1, index) == value);
        assert(cnpy_get_u2(a_out1, index) == cnpy_get_u2(a_out2, index));
        }
        break;
      case CNPY_U4: {
        uint32_t value = cnpy_get_u4(a_in, index);
        printf("%"PRIu32, value);
        fflush(stdout);
        cnpy_set_u4(a_out1, index, value);
        cnpy_set_u4(a_out2, index, value);
        assert(cnpy_get_u4(a_out1, index) == value);
        assert(cnpy_get_u4(a_out1, index) == cnpy_get_u4(a_out2, index));
        }
        break;
      case CNPY_U8: {
        uint64_t value = cnpy_get_u8(a_in, index);
        printf("%"PRIu64, value);
        fflush(stdout);
        cnpy_set_u8(a_out1, index, value);
        cnpy_set_u8(a_out2, index, value);
        assert(cnpy_get_u8(a_out1, index) == value);
        assert(cnpy_get_u8(a_out1, index) == cnpy_get_u8(a_out2, index));
        }
        break;
      case CNPY_F4: {
        float value = cnpy_get_f4(a_in, index);
        printf("%a", value);
        fflush(stdout);
        cnpy_set_f4(a_out1, index, value);
        cnpy_set_f4(a_out2, index, value);
        assert(equal_f(cnpy_get_f4(a_out1, index), value));
        assert(equal_f(cnpy_get_f4(a_out1, index), cnpy_get_f4(a_out2, index)));
        }
        break;
      case CNPY_F8: {
        double value = cnpy_get_f8(a_in, index);
        printf("%la", value);
        fflush(stdout);
        cnpy_set_f8(a_out1, index, value);
        cnpy_set_f8(a_out2, index, value);
        assert(equal_d(cnpy_get_f8(a_out1, index), value));
        assert(equal_d(cnpy_get_f8(a_out1, index), cnpy_get_f8(a_out2, index)));
        }
        break;
      case CNPY_C8: {
        complex float value = cnpy_get_c8(a_in, index);
        printf("%a +%a I", crealf(value), cimagf(value));
        fflush(stdout);
        cnpy_set_c8(a_out1, index, value);
        cnpy_set_c8(a_out2, index, value);
        assert(equal_cf(cnpy_get_c8(a_out1, index), value));
        assert(equal_cf(cnpy_get_c8(a_out1, index), cnpy_get_c8(a_out2, index)));
        }
        break;
      case CNPY_C16: {
        complex double value = cnpy_get_c16(a_in, index);
        printf("%la + %la I", creal(value), cimag(value));
        fflush(stdout);
        cnpy_set_c16(a_out1, index, value);
        cnpy_set_c16(a_out2, index, value);
        assert(equal_cd(cnpy_get_c16(a_out1, index), value));
        assert(equal_cd(cnpy_get_c16(a_out1, index), cnpy_get_c16(a_out2, index)));
        }
        break;
      default:
        assert(false);
    }

    printf("\n");

  } while (cnpy_next_index(a_in, index));
}
