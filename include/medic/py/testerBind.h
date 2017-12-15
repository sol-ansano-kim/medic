#ifndef MEDIC_PY_TESTERBIND_INCLUDED
#define MEDIC_PY_TESTERBIND_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/node.h"
#include "medic/report.h"
#include "medic/tester.h"


void bindTester(pybind11::module);


#endif // MEDIC_PY_TESTERBIND_INCLUDED