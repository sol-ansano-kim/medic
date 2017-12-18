#ifndef MEDIC_PY_PLUGINMANAGER_INCLUDED
#define MEDIC_PY_PLUGINMANAGER_INCLUDED


#include <pybind11/pybind11.h>
#include "medic/pluginManager.h"


void bindPluginManager(pybind11::module);


#endif // MEDIC_PY_PLUGINMANAGER_INCLUDED