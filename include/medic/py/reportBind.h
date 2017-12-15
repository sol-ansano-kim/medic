#ifndef MEDIC_PY_REPORTBIND_INCLUDED
#define MEDIC_PY_REPORTBIND_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/node.h"
#include "medic/report.h"


void bindReport(pybind11::module);


#endif // MEDIC_PY_REPORTBIND_INCLUDED