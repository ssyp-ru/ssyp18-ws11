#ifndef _Q_H_
#define _Q_H_

#include "matrix.h"

typedef struct qbit {
  int measured_value;
  vec_t v;
} qbit_t;

typedef struct qreg {
  uval_t measured_bits;
  uval_t measured_values;
  uval_t nbits;
  vec_t v;
} qreg_t;

typedef struct qgate {
  uval_t nbits;
  mat_t m;
} qgate_t;

qbit_t *qbit_new();
int qbit_apply(qbit_t *bit, qgate_t *gate);
uval_t qbit_measure (qbit_t *bit);

qreg_t *qreg_new(uval_t nbits);
void qreg_init(qreg_t *r, char *str);
int qreg_initi(qreg_t *r, uval_t state);
/* returns 0 on error */
int qreg_apply(qreg_t *reg, qgate_t *gate);
/* returns number of state from 0 to 2^nbits */
uval_t qreg_measure (qreg_t *reg);
uval_t qreg_measure_bit (qreg_t *reg, uval_t bit);
/* print probability distribution */
void qreg_print_probs(qreg_t *reg);

qgate_t *qgate_new(uval_t nbits);
qgate_t *qgate_mul(qgate_t *a, qgate_t *b);
qgate_t *qgate_pow(qgate_t *gate, uval_t n);
qgate_t *qgate_build(int ngates, ...);
qgate_t *qgate_oracle(uval_t *f, uval_t n);

#endif
