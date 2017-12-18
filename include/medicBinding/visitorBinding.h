#ifndef MEDIC_PY_VISITORBIND_INCLUDED
#define MEDIC_PY_VISITORBIND_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/visitor.h"


void bindVisitor(pybind11::module);


#endif // MEDIC_PY_VISITORBIND_INCLUDED