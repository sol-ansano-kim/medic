#ifndef MEDICBINDING_TESTERBINDING_INCLUDED
#define MEDICBINDING_TESTERBINDING_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/node.h"
#include "medic/report.h"
#include "medic/tester.h"


void bindTester(pybind11::module);


#endif // MEDICBINDING_TESTERBINDING_INCLUDED