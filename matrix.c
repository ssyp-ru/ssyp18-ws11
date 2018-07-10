
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"

mat_t *mat_new(uval_t dim){
  mat_t *a = (mat_t*) malloc(sizeof(mat_t) + sizeof(complex_t) * dim * dim);
  a->data = ((void*)a) + sizeof(mat_t);
  a->dim = dim;
  memset(a->data, 0, sizeof(complex_t) * dim * dim);
  return a;
}

vec_t *vec_new(uval_t dim){
  vec_t *a = (vec_t*) malloc(sizeof(vec_t) + sizeof(complex_t) * dim);
  a->data = ((void*)a) + sizeof(vec_t);
  a->dim = dim;
  memset(a->data, 0, sizeof(complex_t) * dim);
  return a;
}

void vec_normalize(vec_t *v) {
  double d = 0.0;
  for (uval_t i = 0; i < v->dim; i++) {
    d += cabs(v->data[i]) * cabs(v->data[i]);
  }
  d = sqrt(d);
  if (d != 0.0) {
    for (uval_t i = 0; i < v->dim; i++) v->data[i] /= d;
  }
}

void mat_print(mat_t *m) {
  for(int i = 0; i < m->dim; i++){
    for(int j = 0; j < m->dim; j++){
        printf("%.1f + %.1f*i ", creal(m->data[i * m->dim + j]), cimag(m->data[i * m->dim + j]));
    }
    printf("\n");
  }
}

void vec_print(vec_t *m) {
  putchar('(');
  for(int i = 0; i < m->dim; i++){
    printf("%.1f + %.1f * i", creal(m->data[i]), cimag(m->data[i]));
    if (i < m->dim - 1){
      printf(", ");
    }
  }
  printf(")\n");
}

mat_t *mat_mul(mat_t *a, mat_t *b, mat_t *res){
    if(a->dim != b->dim) return NULL;

    if (res) {
        if (res->dim != a->dim) return NULL;
    } else {
        res = mat_new(a->dim);
    }

    uval_t dim = res->dim;
    for(int i = 0; i < dim; i++){
      for(int j = 0; j < dim; j++){
        res->data[i * dim + j] = 0;
        for(int k = 0; k < dim; k++){
          res->data[i * dim + j] += a->data[i * dim + k] * b->data[k * dim + j];
        }
      }
    }
    return res;
}

vec_t *mat_vec_mul(mat_t *m, vec_t *v, vec_t *res) {
  if (m->dim != v->dim) return NULL;

  if (res) {
    if (res->dim != v->dim) return NULL;
  } else {
      res = vec_new(v->dim);
  }

  uval_t dim = res->dim;
  complex_t t;

  for(int i = 0; i < dim; i++) {
    t = 0.0;
    for(int k = 0; k < dim; k++) {
      t += m->data[i * dim + k] * v->data[k];
    }
    res->data[i] = t;
  }

  return res;
}

mat_t *mat_mul_kron(mat_t *a, mat_t *b, mat_t *res){
   if(res == NULL){
     res = mat_new(a->dim * b->dim);
   } else {
     if(res->dim != a->dim * b->dim) return NULL;
   }

    uval_t x, y;

    for(uval_t i = 0; i < a->dim; i++){
      for(uval_t j = 0; j < a->dim; j++){
        for(uval_t k = 0; k < b->dim; k++){
          for(uval_t n = 0; n < b->dim; n++){
            x = (i * b->dim) + k;
            y = (j * b->dim) + n;
            res->data[x * res->dim + y] = a->data[i * a->dim + j] * b->data[k * b->dim + n];
          }
        }
      }
    }
    return res;
}

vec_t *vec_mul_kron(vec_t *a, vec_t *b, vec_t *res) {
   if(res == NULL){
     res = vec_new(a->dim * b->dim);
   } else {
     if(res->dim != a->dim * b->dim) return NULL;
   }

    uval_t x;

    for(uval_t i = 0; i < a->dim; i++){
      for(uval_t j = 0; j < b->dim; j++){
          x = (i * b->dim) + j;
          res->data[x] = a->data[i] * b->data[j];
      }
    }

    return res;
}

