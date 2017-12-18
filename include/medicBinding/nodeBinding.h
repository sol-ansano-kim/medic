#ifndef MEDIC_PY_NODEBIND_INCLUDED
#define MEDIC_PY_NODEBIND_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/node.h"


void bindNode(pybind11::module);


#endif // MEDIC_PY_NODEBIND_INCLUDED