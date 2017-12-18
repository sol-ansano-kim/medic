#ifndef MEDICBINDING_KARTEBINDING_INCLUDED
#define MEDICBINDING_KARTEBINDING_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/karte.h"


void bindKarte(pybind11::module);


#endif // MEDICBINDING_KARTEBINDING_INCLUDED