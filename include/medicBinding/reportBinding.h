#ifndef MEDICBINDING_REPORTBINDING_INCLUDED
#define MEDICBINDING_REPORTBINDING_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/node.h"
#include "medic/report.h"


void bindReport(pybind11::module);


#endif // MEDICBINDING_REPORTBINDING_INCLUDED