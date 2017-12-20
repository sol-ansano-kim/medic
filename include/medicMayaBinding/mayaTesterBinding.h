#ifndef MEDICMAYABINDING_MAYATESTERBINDING_INCLUDED
#define MEDICMAYABINDING_MAYATESTERBINDING_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/parameter.h"
#include "medicMaya/mayaNode.h"
#include "medicMaya/mayaReport.h"
#include "medicMaya/mayaTester.h"


void bindMayaTester(pybind11::module);


#endif // MEDICMAYABINDING_MAYATESTERBINDING_INCLUDED