
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "q.h"
#include "gates.h"

#include <stdlib.h>

void random_bit(){
  qreg_t *reg = qreg_new(1);
  qreg_init(reg, "0");
  qgate_t *H = gate_H();
  qreg_apply(reg, H);
  qreg_print_probs(reg);
  printf("Random bit: %d\n", qreg_measure(reg));
  free(reg);
  free(H);
}

void deutsch() {
  qreg_t *reg = qreg_new(2);

  qgate_t *H = gate_H();
  qgate_t *E = gate_E();
  qgate_t *H2 = qgate_mul(H, H);
  qgate_t *HE = qgate_mul(H, E);

  uval_t f[2];
  f[0] = 0; f[1] = 1;
  qgate_t *U = qgate_oracle(f, 1);

  qreg_init(reg, "01");

  qreg_apply(reg, H2);
  qreg_apply(reg, U);
  qreg_apply(reg, HE);

  qreg_print_probs(reg);

  uval_t t = qreg_measure(reg);
  printf("%s\n", (t >> 1) ? "balanced" : "constant");

  free(reg);
  free(H);
  free(E);
  free(H2);
  free(HE);
  free(U);
}

void deutsch_jozsa(uval_t *f, uval_t n) {
  qreg_t *reg = qreg_new(n + 1);

  qgate_t *H1 = gate_H();
  qgate_t *E1 = gate_E();
  qgate_t *Hn = qgate_pow(H1, n);
  qgate_t *H = qgate_mul(Hn, H1);
  qgate_t *HE = qgate_mul(Hn, E1);

  qgate_t *U = qgate_oracle(f, n);

  qreg_initi(reg, 1);
  qreg_apply(reg, H);
  qreg_apply(reg, U);
  qreg_apply(reg, HE);

  qreg_print_probs(reg);

  uval_t m = qreg_measure(reg);
  printf("%s\n", (m >> 1) ? "balanced" : "constant");

  free(reg);
  free(H1);
  free(E1);
  free(Hn);
  free(H);
  free(HE);
  free(U);
}

void swap1() {
  qreg_t *reg = qreg_new(2);
  qreg_init(reg, "01");
  qreg_print_probs(reg);
  qgate_t *c0 = gate_cnot_first();
  qgate_t *c1 = gate_cnot_second();
  printf("perform swap\n");
  qreg_apply(reg, c0);
  qreg_apply(reg, c1);
  qreg_apply(reg, c0);
  qreg_print_probs(reg);
  free(reg);
  free(c0);
  free(c1);
}

void teleport() {
    qreg_t *reg = qreg_new(3);

    qgate_t *H = gate_H(); qgate_t *E = gate_E(); qgate_t *C = gate_cnot_first();
    qgate_t *EHE = qgate_build(3, E, H, E);
    qgate_t *EC  = qgate_build(2, E, C);
    qgate_t *CE  = qgate_build(2, C, E);
    qgate_t *HEE = qgate_build(3, H, E, E);

    qreg_init(reg, "100");
    qreg_apply(reg, EHE);
    qreg_apply(reg, EC);
    qreg_apply(reg, CE);
    qreg_apply(reg, HEE);

    uval_t bit0 = qreg_measure_bit(reg, 0);
    uval_t bit1 = qreg_measure_bit(reg, 1);

    if (bit1) {
      qgate_t *X = gate_X();
      qgate_t *EEX = qgate_build(3, E, E, X);
      qreg_apply(reg, EEX);
      free(X);
      free(EEX);
    }

    if (bit0) {
      qgate_t *Z = gate_Z();
      qgate_t *EEZ = qgate_build(3, E, E, Z);
      qreg_apply(reg, EEZ);
      free(Z);
      free(EEZ);
    }

    printf("%d\n", qreg_measure_bit(reg, 2));

    free(H); free(E); free(C);
    free(EHE); free(CE); free(EC); free(HEE);
    free(reg);
}

int main () {
  srand(time(NULL));

  random_bit();

  deutsch();
  swap1();

  uval_t f[8];
  f[0] = 1; f[1] = 1; f[2] = 0; f[3] = 0;
  f[4] = 0; f[5] = 1; f[6] = 0; f[7] = 1;
  deutsch_jozsa(f, 3);

  uval_t g[8];
  g[0] = 0; g[1] = 0; g[2] = 0; g[3] = 0;
  g[4] = 0; g[5] = 0; g[6] = 0; g[7] = 0;
  deutsch_jozsa(g, 3);

  teleport();

  return EXIT_SUCCESS;
}
