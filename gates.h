#ifndef _GATES_H_
#define _GATES_H_

#include "q.h"

qgate_t *gate_H ();
qgate_t *gate_E ();
qgate_t *gate_Z ();
qgate_t *gate_X ();
qgate_t *gate_Y ();

qgate_t *gate_cnot_first();
qgate_t *gate_cnot_second();

#endif
