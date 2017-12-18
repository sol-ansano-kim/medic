#ifndef MEDICBINDING_VISITORBINDING_INCLUDED
#define MEDICBINDING_VISITORBINDING_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/visitor.h"


void bindVisitor(pybind11::module);


#endif // MEDICBINDING_VISITORBINDING_INCLUDED