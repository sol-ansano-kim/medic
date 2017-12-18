#ifndef MEDIC_PY_PARAMETERBIND_INCLUDED
#define MEDIC_PY_PARAMETERBIND_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/parameter.h"


void bindParameter(pybind11::module);


#endif // MEDIC_PY_PARAMETERBIND_INCLUDED