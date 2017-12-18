#ifndef MEDIC_PY_KARTEBIND_INCLUDED
#define MEDIC_PY_KARTEBIND_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/karte.h"


void bindKarte(pybind11::module);


#endif // MEDIC_PY_KARTEBIND_INCLUDED