#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cnpy.h"

bool equalf(float x1, float x2) {
  if (isnan(x1) && isnan(x2)) {
    return true;
  }
  else if (x1 == 0.0 && x2 == 0.0) {
    return copysignf(1.0, x1) == copysignf(1.0, x2);
  }
  else {
    return x1 == x2;
  }
  return false;
}

bool equald(double x1, double x2) {
  if (isnan(x1) && isnan(x2)) {
    return true;
  }
  else if (x1 == 0.0 && x2 == 0.0) {
    return copysign(1.0, x1) == copysign(1.0, x2);
  }
  else {
    return x1 == x2;
  }
  return false;
}

bool equalcf(complex float x1, complex float x2) {
  return equalf(crealf(x1), crealf(x2)) && equalf(cimagf(x1), cimagf(x2));
}

bool equalcd(complex double x1, complex double x2) {
  return equald(creal(x1), creal(x2)) && equald(cimag(x1), cimag(x2));
}

int main(void) {
  char *fns[] = {
    "f4-be-c.npy",
    "f4-be-f.npy",
    "f4-le-c.npy",
    "f4-le-f.npy",
    "f8-be-c.npy",
    "f8-be-f.npy",
    "f8-le-c.npy",
    "f8-le-f.npy",
    "c8-be-c.npy",
    "c8-be-f.npy",
    "c8-le-c.npy",
    "c8-le-f.npy",
    "c16-be-c.npy",
    "c16-be-f.npy",
    "c16-le-c.npy",
    "c16-le-f.npy",
  };

  for (size_t i = 0; i < sizeof(fns) / sizeof(fns[0]); i += 1) {
    char *fn = fns[i];
    printf(" %s\n", fn);
    cnpy_array arr;
    if (cnpy_open(fn, false, &arr) != CNPY_SUCCESS) {
      cnpy_perror(NULL);
      abort();
    }
    size_t index[CNPY_MAX_DIM];
    cnpy_reset_index(arr, index);
    do {
      if (arr.dtype == CNPY_F4) {
        float value = cnpy_get_f4(arr, index);
             if (index[0] == 0 && index[1] == 0 && index[2] == 0) { assert(equalcf(value, 0.20226619)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 1) { assert(equalcf(value, 0.29630312)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 2) { assert(equalcf(value, 0.36185613)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 3) { assert(equalcf(value, 0.4482345)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 4) { assert(equalcf(value, 0.1890546)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 0) { assert(equalcf(value, 0.96688086)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 1) { assert(equalcf(value, 0.22068219)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 2) { assert(equalcf(value, 0.05797349)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 3) { assert(equalcf(value, 0.07262674)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 4) { assert(equalcf(value, 0.19552772)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 0) { assert(equalcf(value, 0.92491925)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 1) { assert(equalcf(value, 0.56912297)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 2) { assert(equalcf(value, 0.19166313)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 3) { assert(equalcf(value, 0.2682258)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 4) { assert(equalcf(value, 0.08082359)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 0) { assert(equalcf(value, 0.20182598)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 1) { assert(equalcf(value, 0.06345562)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 2) { assert(equalcf(value, 0.5387458)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 3) { assert(equalcf(value, 0.258745)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 4) { assert(equalcf(value, 0.4220618)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 0) { assert(equalcf(value, NAN)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 1) { assert(equalcf(value, -0.0)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 2) { assert(equalcf(value, 0.0)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 3) { assert(equalcf(value, 0.0030913467)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 4) { assert(equalcf(value, 0.050263144)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 0) { assert(equalcf(value, 0.1112716)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 1) { assert(equalcf(value, 0.9368828)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 2) { assert(equalcf(value, 0.71284634)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 3) { assert(equalcf(value, 0.59059834)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 4) { assert(equalcf(value, 0.76151997)); }
        else { assert(false); }
      }
      else if (arr.dtype == CNPY_F8) {
        double value = cnpy_get_f8(arr, index);
             if (index[0] == 0 && index[1] == 0 && index[2] == 0) { assert(equald(value, 0.2022661927765359)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 1) { assert(equald(value, 0.2963031104798771)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 2) { assert(equald(value, 0.36185613079070866)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 3) { assert(equald(value, 0.4482344870039454)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 4) { assert(equald(value, 0.18905459693786564)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 0) { assert(equald(value, 0.9668808620741635)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 1) { assert(equald(value, 0.2206821895359954)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 2) { assert(equald(value, 0.0579734879173458)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 3) { assert(equald(value, 0.07262674017923643)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 4) { assert(equald(value, 0.19552771331435126)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 0) { assert(equald(value, 0.9249192458333583)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 1) { assert(equald(value, 0.5691229587632597)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 2) { assert(equald(value, 0.19166313550681457)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 3) { assert(equald(value, 0.26822578886638215)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 4) { assert(equald(value, 0.08082359577340215)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 0) { assert(equald(value, 0.20182598100469062)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 1) { assert(equald(value, 0.06345561719683768)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 2) { assert(equald(value, 0.5387458342544826)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 3) { assert(equald(value, 0.2587450227759823)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 4) { assert(equald(value, 0.4220618020571275)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 0) { assert(equald(value, NAN)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 1) { assert(equald(value, -0.0)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 2) { assert(equald(value, 0.0)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 3) { assert(equald(value, 0.0030913466127576728)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 4) { assert(equald(value, 0.05026314271925025)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 0) { assert(equald(value, 0.11127160010859283)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 1) { assert(equald(value, 0.9368828156617994)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 2) { assert(equald(value, 0.7128463320562705)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 3) { assert(equald(value, 0.5905983682737793)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 4) { assert(equald(value, 0.7615199569672999)); }
        else { assert(false); }
      }
      else if (arr.dtype == CNPY_C8) {
        complex float value = cnpy_get_c8(arr, index);
             if (index[0] == 0 && index[1] == 0 && index[2] == 0) { assert(equalcf(value, 0.20226619 + 0.9111516 * I)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 1) { assert(equalcf(value, 0.29630312 + 0.94427335 * I)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 2) { assert(equalcf(value, 0.36185613 + 0.91568744 * I)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 3) { assert(equalcf(value, 0.4482345 + 0.056531955 * I)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 4) { assert(equalcf(value, 0.1890546 + 0.16049603 * I)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 0) { assert(equalcf(value, 0.96688086 + 0.5826273 * I)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 1) { assert(equalcf(value, 0.22068219 + 0.17008537 * I)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 2) { assert(equalcf(value, 0.05797349 + 0.3911584 * I)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 3) { assert(equalcf(value, 0.07262674 + 0.88716334 * I)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 4) { assert(equalcf(value, 0.19552772 + 0.96390355 * I)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 0) { assert(equalcf(value, 0.92491925 + 0.3310075 * I)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 1) { assert(equalcf(value, 0.56912297 + 0.21229534 * I)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 2) { assert(equalcf(value, 0.19166313 + 0.05848843 * I)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 3) { assert(equalcf(value, 0.2682258 + 0.46625292 * I)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 4) { assert(equalcf(value, 0.08082359 + 0.867439 * I)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 0) { assert(equalcf(value, 0.20182598 + 0.41335192 * I)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 1) { assert(equalcf(value, 0.06345562 + 0.5141957 * I)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 2) { assert(equalcf(value, 0.5387458 + 0.7608628 * I)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 3) { assert(equalcf(value, 0.258745 + 0.3519256 * I)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 4) { assert(equalcf(value, 0.4220618 + 0.910776 * I)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 0) { assert(equalcf(value, NAN + 0.0 * I)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 1) { assert(equalcf(value, (complex float) -0.0f)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 2) { assert(equalcf(value, 0.0 + 0.0 * I)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 3) { assert(equalcf(value, 0.0030913467 + 0.8728059 * I)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 4) { assert(equalcf(value, 0.050263144 + 0.58477527 * I)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 0) { assert(equalcf(value, 0.1112716 + 0.8796717 * I)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 1) { assert(equalcf(value, 0.9368828 + 0.32567364 * I)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 2) { assert(equalcf(value, 0.71284634 + 0.8451681 * I)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 3) { assert(equalcf(value, 0.59059834 + 0.35566688 * I)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 4) { assert(equalcf(value, 0.76151997 + 0.7925646 * I)); }
        else { assert(false); }
      }
      if (arr.dtype == CNPY_C16) {
        complex double value = cnpy_get_c16(arr, index);
             if (index[0] == 0 && index[1] == 0 && index[2] == 0) { assert(equalcd(value, 0.2022661927765359+0.911151576381147*I)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 1) { assert(equalcd(value, 0.2963031104798771+0.9442733515553428*I)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 2) { assert(equalcd(value, 0.36185613079070866+0.915687433667723*I)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 3) { assert(equalcd(value, 0.4482344870039454+0.056531955645364285*I)); }
        else if (index[0] == 0 && index[1] == 0 && index[2] == 4) { assert(equalcd(value, 0.18905459693786564+0.1604960256972614*I)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 0) { assert(equalcd(value, 0.9668808620741635+0.582627305520001*I)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 1) { assert(equalcd(value, 0.2206821895359954+0.17008537585334793*I)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 2) { assert(equalcd(value, 0.0579734879173458+0.3911584129265795*I)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 3) { assert(equalcd(value, 0.07262674017923643+0.8871633401903899*I)); }
        else if (index[0] == 0 && index[1] == 1 && index[2] == 4) { assert(equalcd(value, 0.19552771331435126+0.9639035549815377*I)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 0) { assert(equalcd(value, 0.9249192458333583+0.3310075212325322*I)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 1) { assert(equalcd(value, 0.5691229587632597+0.21229533298877334*I)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 2) { assert(equalcd(value, 0.19166313550681457+0.05848842738185045*I)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 3) { assert(equalcd(value, 0.26822578886638215+0.4662529371923042*I)); }
        else if (index[0] == 0 && index[1] == 2 && index[2] == 4) { assert(equalcd(value, 0.08082359577340215+0.867438972231096*I)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 0) { assert(equalcd(value, 0.20182598100469062+0.4133519175328306*I)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 1) { assert(equalcd(value, 0.06345561719683768+0.5141957086015784*I)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 2) { assert(equalcd(value, 0.5387458342544826+0.7608628302375843*I)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 3) { assert(equalcd(value, 0.2587450227759823+0.3519256226804538*I)); }
        else if (index[0] == 1 && index[1] == 0 && index[2] == 4) { assert(equalcd(value, 0.4220618020571275+0.9107759975080193*I)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 0) { assert(equalcd(value, NAN+0.0*I)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 1) { assert(equalcd(value, (complex double) -0.0)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 2) { assert(equalcd(value, 0.0)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 3) { assert(equalcd(value, 0.0030913466127576728+0.8728058725334997*I)); }
        else if (index[0] == 1 && index[1] == 1 && index[2] == 4) { assert(equalcd(value, 0.05026314271925025+0.584775261507355*I)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 0) { assert(equalcd(value, 0.11127160010859283+0.8796717133291572*I)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 1) { assert(equalcd(value, 0.9368828156617994+0.3256736313689854*I)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 2) { assert(equalcd(value, 0.7128463320562705+0.8451681172973272*I)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 3) { assert(equalcd(value, 0.5905983682737793+0.3556668740391734*I)); }
        else if (index[0] == 1 && index[1] == 2 && index[2] == 4) { assert(equalcd(value, 0.7615199569672999+0.792564580456437*I)); }
        else { assert(false); }
      }
    } while (cnpy_next_index(arr, index));
    printf("  ok.\n");
  }
}
