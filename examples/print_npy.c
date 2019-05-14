/*
 * print_npy.c - print metadata and contents of .npy files in human-readable form.
 *
 * Copyright (C) 2018, 2019 Lukas Himbert
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License only.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include "cnpy.h"

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

  /* Open file */
  cnpy_array a;
  if (cnpy_open(fn, true, &a) != CNPY_SUCCESS) {
    cnpy_perror("Input file not opened");
    return EXIT_SUCCESS;
  }

  /* Print header information */
  printf("dtype: ");
  switch(a.dtype) {
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
  switch (a.byte_order) {
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
  print_index(a.n_dim, a.dims);
  printf("\n");
  switch (a.order) {
    case CNPY_FORTRAN_ORDER:
      printf("Fortran order (column major)\n\n");
      break;
    case CNPY_C_ORDER:
      printf("C order (row major)\n\n");
      break;
    default:
      assert(false);
  }

  /* Now print the contents. */
  size_t index[CNPY_MAX_DIM];
  cnpy_reset_index(a, index);
  do {
    print_index(a.n_dim, index);
    printf(": ");
    switch (a.dtype) {
      case CNPY_B:
        printf("%s\n", cnpy_get_b(a, index)? "true" : "false");
        break;
      case CNPY_I1:
        printf("%"PRIi8"\n", cnpy_get_i1(a, index));
        break;
      case CNPY_I2:
        printf("%"PRIi16"\n", cnpy_get_i2(a, index));
        break;
      case CNPY_I4:
        printf("%"PRIi32"\n", cnpy_get_i4(a, index));
        break;
      case CNPY_I8:
        printf("%"PRIi64"\n", cnpy_get_i8(a, index));
        break;
      case CNPY_U1:
        printf("%"PRIu8"\n", cnpy_get_u1(a, index));
        break;
      case CNPY_U2:
        printf("%"PRIu16"\n", cnpy_get_u2(a, index));
        break;
      case CNPY_U4:
        printf("%"PRIu32"\n", cnpy_get_u4(a, index));
        break;
      case CNPY_U8:
        printf("%"PRIu64"\n", cnpy_get_u8(a, index));
        break;
      case CNPY_F4:
        printf("%8g\n", cnpy_get_f4(a, index));
        break;
      case CNPY_F8:
        printf("%17lg\n", cnpy_get_f8(a, index));
        break;
      case CNPY_C8: {
        complex float value = cnpy_get_c8(a, index);
        printf("%8g + %8g * I\n", crealf(value), cimagf(value));
        }
        break;
      case CNPY_C16: {
        complex float value = cnpy_get_c16(a, index);
        printf("%17g + %17g * I\n", creal(value), cimag(value));
        }
        break;
      default:
        assert(false);
    }
  } while (cnpy_next_index(a, index));

  return EXIT_SUCCESS;
}
