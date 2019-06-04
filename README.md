`cnpy.h` - a C library for .npy files
=====================================


Summary
-------

`cnpy.h` is a C99 single-header library to read and write numerical data in the numpy `.npy` format.
Files are always loaded with mmap(2), so they may be larger than main memory size; it also means that data is only copied to main memory as needed (if only a small number of entries is accessed, only a small amount is loaded into memory).
The library performs no dynamic memory management except a single mmap per file; it never calls malloc(3).

Build status: [![builds.sr.ht status](https://builds.sr.ht/~quf/cnpy.svg)](https://builds.sr.ht/~quf/cnpy?)


Examples
--------

- Open an existing `.npy` file with 2 dimensions and complex entries, print the entries, and then change them (in memory only):

```C
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
```

- Create a new `.npy` file and save some data inside:

```C
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
```

- For a more complex example (where the data type and number of dimensions are not know in advance), see the file [`examples/print_npy.c`](https://git.sr.ht/~quf/cnpy/tree/master/examples/print_npy.c), which prints the header and content of an `.npy` file in human-readable form.


Standards and Compatibility
---------------------------

`cnpy.h` is almost POSIX.1 (2008), C99 and C11 conformant.

It requires the nonstandard functions `__builtin_add_overflow()` and `__builtin_mul_overflow()`, and the `#pragma once` preprocessor directive; both gcc and clang support this.

`cnpy.h` requires `8` bit `char`s, IEEE 754 representation of `float` and `double`, and will work on little endian and big endian architectures.

No other dependencies are required.

If compiled with C11 support, there are a few additional `_Static_assert`s.

`cnpy.h` supports a subset of the [`.npy` format specification](https://docs.scipy.org/doc/numpy-1.14.0/neps/npy-format.html).
Version 1.0 is supported for reading and writing.
Version 2.0 is supported for reading, but not writing.
Writing version 2.0 will only become necessary once the size of your data exceeds approximately 10^6500 bytes \[sic\].
Not all possible dtypes are supported (for a list of supported dtypes, see section Supported data types).
In particular, strings and Python objects are not supported.

The library is known to work on OpenBSD 6.4 amd64 (compiled with clang), Gentoo Linux amd64 (gcc and clang), Arch Linux amd64 (gcc and clang), and Alpine Linux armhf (gcc).


Caveats
-------

While mmap(2) has advantages compared to read(2), it can also lead to unexpected problems.
You should expect errors if another process changes the file size while it is opened.
When the file system is unreliable (e. g. a remote network file system with an unreliable network connection) the process may receive a SIGBUS signal at any time an array is accessed (because an unmapped page may need to be loaded from the backing file when the backing file is unavailable).
No problems are expected when the backing file lives on a reliable file system and no other process (or thread) accesses the file.
For more information, see msync(3).

Only reading and writing `.npy` files is supported.
Archive files (ending `.npz`) are not supported.
Fancy calculations like `arr1 * arr2 + arr3` are outside of the scope of this library.

Testing is not as thorough as one would hope.
In particular, this library has only been tested on little endian architectures (amd64 and arm7), though it should also work on big endian architectures.
Further, there are no automated tests to make sure that ill-formatted files are rejected.

Not all data types are supported (for a list of supported datatypes, see section Supported data types).
In particular, strings and Python objects are not supported.

There are no performance benchmarks for this library.
Speed was not a high priority when writing this library.
As an example, opening an uncached file with 10^9 doubles, adding all elements, and printing the result takes around 85s on the author's laptop.


Versions
--------

`cnpy.h` versioning follows [semanting versioning v2.0.0](https://semver.org/).

The current project version is 1.0.2.


Thread-safety
-------------

`cnpy.h` is neither reentrant nor thread-safe.
It should not be used by multiple threads of the same process at the same time.

It is thread-safe (multiple threads may parse different files at the same time) iff it is compiled as C11 code and the `<threads.h>` library is available.
Before relying on this behaviour, you should check that `CNPY_THREADSAFE` is defined.


Error handling
--------------

Some user facing functions return a variable of type `cnpy_status`.
If the return value is `CNPY_SUCCESS`, the function was executed successfully; otherwise, there was an error.

In case of an error, the string `cnpy_error_str` contains more specific information.
This string is thread-local.
Users should not change this string.

The function `cnpy_perror()` may be used to print error messages to `stderr` if an `cnpy.h` error occured, analogous to `perror(3)`.
If `str` is not `NULL`, it is printed as a user specified error string.

The function `cnpy_error_reset()` resets the error message.
It should be called before calling another function from `cnpy.h` if an error occured and was handled.


Supported data types
--------------------

Supported data types:

 - Boolean values
 - Signed integers (sizes 1, 2, 4, 8 bytes)
 - Unsigned integers (sizes 1, 2, 4, 8 bytes)
 - Floats (sizes 4, 8 bytes)
 - Complex floats (sizes 8, 16 bytes)

For each datatype with size larger than one byte, both little endianness and big endianness is supported.
For datatypes of size one byte, endianness is irrelevant.
"host endianness" is intentionally not supported; this will not change in future versions because it is a terrible idea.


Public API
----------

Types:

- `cnpy_status`:
  `cnpy.h` return value type.
  Possible values:
  - `CNPY_SUCCESS`: There was no error.
  - `CNPY_ERROR_FILE`: There was an error related to file handling (e. g. the file does not exist)
  - `CNPY_ERROR_MMAP`: There was an error related to mmap() (i. e. mmap() or munmap() failed)
  - `CNPY_ERROR_FORMAT`: There was an error related to the file format (e. g. the file is corrupted, has an unsupported dtype, or has too many dimensions)

- `cnpy_array`:
  The array datatype.
  Is a struct with members `size_t n_dim` (number of dimensions), `size_t dims[n_dim]` (shape), `cnpy_dtype dtype` (datatype), `cnpy_byte_order byte_order` (byte order/endianness), `cnpy_flat_order order` (serialization order; column-major or row-major).
  These should not be set by the user, but they may be read.

- `cnpy_dtype`:
  `.npy` element datatypes.
  Possible values: `CNPY_B`, `CNPY_I1`, `CNPY_I2`, `CNPY_I4`, `CNPY_I8` `CNPY_U1`, `CNPY_U2`, `CNPY_U4`, `CNPY_U8`, `CNPY_F4`, `CNPY_F8`, `CNPY_C8`, `CNPY_C16`.

- `cnpy_byte_order`:
  Byte order / endianness.
  Possible values: `CNPY_LE` (little endian), `CNPY_BE` (big endian), `CNPY_NE` (no endianness / single byte; only applicable for `CNPY_BOOL`, `CNPY_INT8`, `CNPY_UINT8`).
  Note that "host endianness" ("=" in `.npy` format) is not supported and will not be supported in the future.

- `cnpy_flat_order`:
  Serialization / Flattening order.
  Possible values: `CNPY_FORTRAN_ORDER` (Fortran order / column major), `CNPY_C_ORDER` (C order / row major).


Functions:

- `void cnpy_perror(char *str)`:
  Print the user-supplied error string `str` and the `cnpy.h`-internal error string to `stderr`.
  `str` may be NULL or zero-length, in which case only the `cnpy.h` error string is printed.

- `void cnpy_error_reset(void)`:
  reset the internal `cnpy.h` error string after an error occured.

- `cnpy_status open(const char * const fn, bool writable, cnpy_array *arr)`:
  Open an existing `.npy` file with file name `fn` as `*arr`.
  If `writable` is `true`, then changes to the array will be persistent.
  If `writable` is `false` the array elements can also be changed, but this will not change the file.
  On success, returns `CNPY_SUCCESS`.
  On failure, returns a different value, and `*arr` is unchanged.

- `cnpy_status create(const char * const fn, cnpy_byte_order byte_order, cnpy_dtype dtype, cnpy_flat_order order, size_t n_dim, const size_t const *dims, cnpy_array *arr)`:
  Create a new `.npy` file with file name `fn` as `*arr`.
  `byte_order` is the desired byte order of the new file.
  `dtype` is the desired data type of the new file.
  `order` is the desired array serialization order (row major/C order or column major / Fortran order).
  `n_dim` is the number of dimensions of the array (must be smaller than or equal to `CNPY_MAX_DIM`).
  `dims` is the desired shape of the array; `dims[i]` must be positive for each `i < n_dim`.
  The entries of the array will be initialized to 0.
  `n_dim` must be smaller than or equal to `CNPY_MAX_DIM`.
  If `fn` is `NULL` and `MAP_ANONYMOUS` is available, an anonymous mapping is created (i. e., the array resides in memory only and changes to it are not written to a file).

- `cnpy_status cnpy_close(cnpy_array *arr)`:
  Close a previously opened or created array `*arr`.
  Afterwards, `*arr` must not be accessed again (until another file is opened or created into it);
  On success, returns `CNPY_SUCCESS`.
  On failure, returns a different value, and `*arr` is unchanged.

- `bool cnpy_get_b(const cnpy_array arr, const size_t * const index)`:
  Get the value of the element of `arr` given by `index`.
  `index` must be in range of the dimensions of `arr`.
  `arr` must be a valid `cnpy_array` with `dtype` `CNPY_B`.
  Serialization order is taken into account and byte order conversion is performed if necessary.

- `void cnpy_set_b(cnpy_array arr, const size_t *const index, bool x)`:
  Set element of `arr` given by `index` to `val`.
  `index` must be in range of the dimensions of `arr`.
  `arr` must be a valid `cnpy_array` with `dtype` `CNPY_B`.
  Serialization order is taken into account and byte order conversion is performed if necessary.

- `int8_t cnpy_get_i1(const cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_i1(cnpy_array arr, const size_t * const index, int8_t x)`
  `int16_t cnpy_get_i2(const cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_i2(cnpy_array arr, const size_t * const index, int16_t x)`
  `int32_t cnpy_get_i4(const cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_i4(cnpy_array arr, const size_t * const index, int32_t x)`
  `int64_t cnpy_get_i8(const cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_i8(cnpy_array arr, const size_t * const index, int64_t x)`
  `uint8_t cnpy_get_u1(const cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_u1(cnpy_array arr, const size_t * const index, uint8_t x)`
  `uint16_t cnpy_get_u2(cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_u2(cnpy_array arr, const size_t * const index, uint16_t x)`
  `uint32_t cnpy_get_u4(const cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_u4(cnpy_array arr, const size_t * const index, uint32_t x)`
  `uint64_t cnpy_get_u8(const cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_u8(cnpy_array arr, const size_t * const index, uint64_t x)`
  `float cnpy_get_f4(const cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_f4(cnpy_array arr, const size_t * const index, float x)`
  `double cnpy_get_f8(const cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_f8(cnpy_array arr, const size_t * const index, double x)`
  `complex float cnpy_get_c8(const cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_c8(const cnpy_array arr, const size_t * const index, complex float x)`
  `complex double cnpy_get_c16(const cnpy_array arr, const size_t * const index)`,
  `void cnpy_set_c16(const cnpy_array arr, const size_t * const index, complex double x)`:
  Other accessors, analogous to `cnpy_get_b()` and `cnpy_set_b()`.

- `void cnpy_reset_index(const cnpy_array arr, size_t *index)`:
  Set the first `arr.n_dim` entries of `index` to zero.

- `bool cnpy_next_index(const cnpy_array arr, size_t *index)`:
  Tries to set `index` to the next-larger multi-index for `arr` (taking into account `arr.order`).
  For all `i < arr.n_dim`, `index[i] < arr.dims[i]` must be true.
  If `index` is the maximum possible index, `index` is left unchanged and `false` is returned; otherwise `index` is set to the next higher multi-index for `arr` and `true` is returned.
  This can be used to iterate through all elements of a `cnpy_array` in a `do {} while();` loop.


Preprocessor variables:

- `CNPY_MAX_DIM`:
  The maximum number of dimensions a `.npy` array may have.
  `4` by default.
  May be overridden by the user (e. g. using a compile flag like `-DCNPY_MAX_DIM 5` or `#define CNPY_MAX_DIM 5` before including `cnpy.h`).
  Assuming `sizeof(size_t) == 8`, the maximum possible value is approximately 2900.
  Note that increasing this value increases the size of the `cnpy_array` type.

- `CNPY_THREADSAFE`:
  If defined, the library is threadsafe.
  If undefined, it is not.

All other functions, types, etc. defined by `cnpy.h` have names starting with `cnpy_`.
They should not be used directly.


Changelist
----------

- 1.0.2 Changes from 1.0.0:
  - Some typos fixed in readme.md
  - Fixed a bug which rejected technically valid files


License
-------

Copyright (C) 2018, 2019 Lukas Himbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License only.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.


Similar work
------------

The only other implementation of the `.npy` format in C the author of this library could find is [c_npy](https://github.com/oysteijo/c_npy).
It does not offer accessors and is "alpha stage" according to its author.
There is also a [blog post on implementing the .npy format in C](https://jcastellssala.com/2014/02/01/npy-in-c/), but it is also very limited in scope.


Developer info
--------------

This section is intended for people who want to change the library, not for people who merely need to use it in their own program.

Conventions:

- "magic string" refers to the 6-byte string "\x93NUMPY" at the beginning of each npy file.
- "preheader" refers to the first 10 or 12 bytes at the beginning of each npy file (including the magic string, the 2 byte version, and the 2 or 4 byte header size)
- "header" refers to the python dictionary with keys "descr", "fortran_order", and "shape" (including padding and excluding the preheader)
- "full header" refers to the preheader and header
- "raw data" refers to the entire file (as a byte string)
- "data" refers to the raw data excluding the full header (i. e., only the content of the array; again, as a byte string)

Parsing follows more or less a stateful recursive descent strategy with fairly obvious mapping between the parser functions and the `.npy` grammar.


TODO
----

- Test on a big-endian architecture.
- Maybe write vararg accessors?
- pread(2) instead of mmap(2) may be worth a try.
