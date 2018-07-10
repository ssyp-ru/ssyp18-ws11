
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "q.h"

static uval_t rand_with_probs(uval_t n, double *probs) {
   int r = rand();
   double x = (1.0/RAND_MAX) * r;

   double sum = 0;

   for(int i = 0; i < n - 1; i++) {
      sum += probs[i];
      if (x <= sum){
        return i;
      }
    }

    return n - 1;
}

void print_binary(int nbits, uval_t val) {
  uval_t mask = 1 << (nbits - 1);
  for (int i = 0; i < nbits; i++) {
    if (val & mask) putchar('1'); else putchar('0');
    mask >>= 1;
  }
}

qbit_t *qbit_new(uval_t nbits) {
    qbit_t *b = (qbit_t*) malloc(sizeof(qreg_t) + sizeof(complex_t) * 2);
    b->measured_value = -1;
    b->v.dim = 2;
    b->v.data = ((void*)b) + sizeof(qbit_t);
    memset(b->v.data, 0, sizeof(complex_t) * 2);
    return b;
}

int qbit_apply(qbit_t *bit, qgate_t *gate) {
  if(gate->nbits > 1){
    return 0;
  }
  vec_t *res = vec_new(bit->v.dim);
  mat_vec_mul(&gate->m, &bit->v, res);
  memcpy(bit->v.data, res->data, sizeof(complex_t) * res->dim);
  free(res);
  return 1;
}

uval_t qbit_measure (qbit_t *bit) {
    if(bit->measured_value == -1){
      double probs[2];
      probs[0] = cabs(bit->v.data[0]) * cabs(bit->v.data[0]);
      probs[1] = cabs(bit->v.data[1]) * cabs(bit->v.data[1]);
      uval_t t = rand_with_probs(2, probs);
      bit->measured_value = t;
    }

    return (uval_t)bit->measured_value;
}

qreg_t *qreg_new(uval_t nbits) {
    uval_t dim = 1 << nbits;
    qreg_t *r = (qreg_t*) malloc(sizeof(qreg_t) + sizeof(complex_t) * dim);
    r->nbits = nbits;
    r->measured_bits = 0;
    r->measured_values = 0;
    r->v.dim = dim;
    r->v.data = ((void*)r) + sizeof(qreg_t);
    return r;
}

void qreg_init(qreg_t *r, char *str) {
  if (str == NULL) return;

  uval_t state = 0;

  for (int i = 0; i < strlen(str); i++) {
    state *= 2;
    if(str[i] == '1')  state += 1;
  }

  qreg_initi(r, state);
}

int qreg_initi(qreg_t *r, uval_t state) {
    memset(r->v.data, 0, sizeof(complex_t) * r->v.dim);
    if (state >= r->v.dim) return 0;
    r->v.data[state] = 1.0;
    return 1;
}

qreg_t *qreg_mul(qreg_t *a, qreg_t *b) {
    qreg_t *res = qreg_new(a->nbits + b->nbits);
    vec_mul_kron(&a->v, &b->v, &res->v);
    return res;
}

int qreg_apply(qreg_t *reg, qgate_t *gate) {
  if(reg->nbits != gate->nbits){
    return 0;
  }

  vec_t *res = vec_new(reg->v.dim);
  mat_vec_mul(&gate->m, &reg->v, res);
  memcpy(reg->v.data, res->data, sizeof(complex_t) * res->dim);
  free(res);
  return 1;
}

uval_t qreg_measure (qreg_t *reg) {
    uval_t all_set = (1 << reg->nbits) - 1;

    if (reg->measured_bits == all_set) {
        return reg->measured_values;
    }

    reg->measured_bits = all_set;

    uval_t n = reg->v.dim;

    double *probs = malloc(sizeof(double)*n);
    for(int i = 0; i < n; i++) {
        probs[i]= cabs(reg->v.data[i]) * cabs(reg->v.data[i]);
    }

    uval_t t = rand_with_probs(n, probs);

    free(probs);
    reg->measured_values = t;

    return t;
}

uval_t qreg_measure_bit (qreg_t *reg, uval_t bit) {
    if (reg->nbits <= bit) return 0;
    bit = reg->nbits - 1 - bit;
    uval_t mask = 1 << bit;

    if (reg->measured_bits & mask) {
        return (reg->measured_values & mask) ? 1 : 0;
    }

    reg->measured_bits |= 1 << bit;

    uval_t n = reg->v.dim;
    double *probs = malloc(sizeof(double)*n);
    for(int i = 0; i < n; i++) {
        probs[i]= cabs(reg->v.data[i]) * cabs(reg->v.data[i]);
    }

    uval_t t = rand_with_probs(n, probs);
    free(probs);

    for (uval_t i = 0; i < reg->v.dim; i++) {
      if ((i & mask) != (t & mask)) reg->v.data[i] = 0.0;
    }

    vec_normalize(&reg->v);

    reg->measured_values |= t & mask;

    return (t & mask) ? 1 : 0;
}

void qreg_print_probs(qreg_t *reg) {
  for (uval_t i = 0; i < reg->v.dim; i++) {
    print_binary(reg->nbits, i);
    printf(": %f\n", creal(reg->v.data[i]) * creal(reg->v.data[i]));
  }
}

qgate_t *qgate_new(uval_t nbits) {
    uval_t dim = 1 << nbits;
    qgate_t *g = (qgate_t*) malloc(sizeof(qgate_t) + sizeof(complex_t) * dim * dim);
    g->nbits = nbits;
    g->m.dim = dim;
    g->m.data = ((void*)g) + sizeof(qgate_t);
    memset(g->m.data, 0, sizeof(complex_t) * dim * dim);
    return g;
}

qgate_t *qgate_mul(qgate_t *a, qgate_t *b){
  qgate_t *res = qgate_new(a->nbits + b->nbits);
  mat_mul_kron(&a->m, &b->m, &res->m);
  return res;
}

qgate_t *qgate_pow(qgate_t *gate, uval_t n) {
  if (n == 1) return gate;
  if (n == 2) return qgate_mul(gate, gate);

  mat_t *m = &gate->m, *t;
  for (unsigned int i = 2; i <= n; i++) {
    t = m;
    m = mat_mul_kron(m, &gate->m, NULL);
    if (t != &gate->m) free(t);
  }

  qgate_t *res = qgate_new(gate->nbits * n);
  memcpy(res->m.data, m->data, sizeof(complex_t) * m->dim * m->dim);
  free(m);

  return res;
}

qgate_t *qgate_build(int ngates, ...) {
  if (ngates <= 0) return NULL;

  va_list argptr;

  va_start(argptr, ngates);
  qgate_t *gate = va_arg(argptr, qgate_t *), *t = NULL;

  for (int i = 1; i < ngates; i++) {
    t = gate;
    gate = qgate_mul(gate, va_arg(argptr, qgate_t *));
    if (i > 1) free(t);
  }

  va_end(argptr);

  return gate;
}

qgate_t *qgate_oracle(uval_t *f, uval_t n) {
  qgate_t *U = qgate_new(n + 1);

  uval_t in, out, x, y;
  for (in = 0; in < U->m.dim; in++) {
    x = in >> 1;
    y = in & 1;
    out = (in & ~1) | (y ^ f[x]);
    U->m.data[in * U->m.dim + out] = 1;
  }

  return U;
}

