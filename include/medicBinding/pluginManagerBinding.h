#ifndef MEDICBINDING_PLUGINMANAGERBINDING_INCLUDED
#define MEDICBINDING_PLUGINMANAGERBINDING_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/pluginManager.h"


void bindPluginManager(pybind11::module);


#endif // MEDICBINDING_PLUGINMANAGERBINDING_INCLUDED