#ifndef MEDICBINDING_PARAMETERBINDING_INCLUDED
#define MEDICBINDING_PARAMETERBINDING_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/parameter.h"


void bindParameter(pybind11::module);


#endif // MEDICBINDING_PARAMETERBINDING_INCLUDED