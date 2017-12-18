#include "medic/py/pluginManagerBind.h"
#include <pybind11/stl.h>


using namespace pybind11;
using namespace MEDIC;


void bindPluginManager(pybind11::module mod)
{
    class_<MdPluginManager, std::unique_ptr<MdPluginManager, nodelete>>(mod, "PluginManager")
        .def_static("Instance", &MdPluginManager::Instance)
        .def("reloadPlugins", &MdPluginManager::reloadPlugins)
        .def("getKarte", &MdPluginManager::getKarte)
        .def("getTester", &MdPluginManager::getTester)
        .def("getKarteNames", &MdPluginManager::getKarteNames)
        .def("getTesterNames", &MdPluginManager::getTesterNames);
}
