#ifndef MEDICBINDING_NODEBINDING_INCLUDED
#define MEDICBINDING_NODEBINDING_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/node.h"


void bindNode(pybind11::module);


#endif // MEDICBINDING_NODEBINDING_INCLUDED