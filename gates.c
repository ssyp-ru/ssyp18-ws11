
#include <stdlib.h>
#include <math.h>

#include "q.h"

qgate_t *gate_H() {
    qgate_t *H = qgate_new(1);

    const double root2 = sqrt(2);
    H->m.data[0] = 1.0/root2; H->m.data[1] = 1.0/root2;
    H->m.data[2] = 1.0/root2; H->m.data[3] = -1.0/root2;

    return H;
}

qgate_t *gate_E() {
    qgate_t *E = qgate_new(1);

    E->m.data[0] = 1.0; E->m.data[1] = 0.0;
    E->m.data[2] = 0.0; E->m.data[3] = 1.0;

    return E;
}

qgate_t *gate_X() {
    qgate_t *X = qgate_new(1);

    X->m.data[0] = 0.0; X->m.data[1] = 1.0;
    X->m.data[2] = 1.0; X->m.data[3] = 0.0;
    return X;
}

qgate_t *gate_Y() {
    qgate_t *Y = qgate_new(1);

    Y->m.data[0] = 0.0; Y->m.data[1] = -I;
    Y->m.data[2] = I;   Y->m.data[3] = 0.0;

    return Y;
}

qgate_t *gate_Z() {
    qgate_t *Z = qgate_new(1);

    Z->m.data[0] = 1.0; Z->m.data[1] = 0.0;
    Z->m.data[2] = 0.0; Z->m.data[3] = -1.0;

    return Z;
}

qgate_t *gate_cnot_first() {
    qgate_t *N = qgate_new(2);

    N->m.data[0]  = 1.0; N->m.data[1]  = 0.0;  N->m.data[2]  = 0.0; N->m.data[3]  = 0.0;
    N->m.data[4]  = 0.0; N->m.data[5]  = 1.0;  N->m.data[6]  = 0.0; N->m.data[7]  = 0.0;
    N->m.data[8]  = 0.0; N->m.data[9]  = 0.0;  N->m.data[10] = 0.0; N->m.data[11] = 1.0;
    N->m.data[12] = 0.0; N->m.data[13] = 0.0;  N->m.data[14] = 1.0; N->m.data[15] = 0.0;

    return N;
}

qgate_t *gate_cnot_second() {
    qgate_t *N = qgate_new(2);

    N->m.data[0]  = 1.0; N->m.data[1]  = 0.0;  N->m.data[2]  = 0.0; N->m.data[3]  = 0.0;
    N->m.data[4]  = 0.0; N->m.data[5]  = 0.0;  N->m.data[6]  = 0.0; N->m.data[7]  = 1.0;
    N->m.data[8]  = 0.0; N->m.data[9]  = 0.0;  N->m.data[10] = 1.0; N->m.data[11] = 0.0;
    N->m.data[12] = 0.0; N->m.data[13] = 1.0;  N->m.data[14] = 0.0; N->m.data[15] = 0.0;

    return N;
}
