#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <complex.h>

typedef unsigned long uval_t;

typedef double complex complex_t;

typedef struct vec {
  uval_t dim;
  complex_t *data;
} vec_t;

typedef struct mat {
  uval_t dim;
  complex_t *data;
} mat_t;

mat_t *mat_new(uval_t dim);
vec_t *vec_new(uval_t dim);

void vec_normalize(vec_t *v);

mat_t *mat_mul(mat_t *a, mat_t *b, mat_t *res);
vec_t *mat_vec_mul(mat_t *m, vec_t *v, vec_t *res);

mat_t *mat_mul_kron(mat_t *a, mat_t *b, mat_t *res);
vec_t *vec_mul_kron(vec_t *a, vec_t *b, vec_t *res);

void mat_print(mat_t *m);
void vec_print(vec_t *v);

#endif
