#include "medic/py/parameterBind.h"
#include "pybind11/operators.h"


using namespace pybind11;
using namespace MEDIC;


void bindParameter(pybind11::module mod)
{
    pybind11::enum_<MdParamType>(mod, "ParamTypes")
        .value("MdNull", MdParamType::MdNull)
        .value("MdBool", MdParamType::MdBool)
        .value("MdInt", MdParamType::MdInt)
        .value("MdFloat", MdParamType::MdFloat)
        .value("MdString", MdParamType::MdString)
        .value("MdBoolArray", MdParamType::MdBoolArray)
        .value("MdIntArray", MdParamType::MdIntArray)
        .value("MdFloatArray", MdParamType::MdFloatArray)
        .value("MdStringArray", MdParamType::MdStringArray)
        .export_values();

    class_<MdParamContainer>(mod, "ParamContainer")
        .def(init())
        .def("append", static_cast<bool (MdParamContainer::*)(std::string, std::string, MdParamType, bool)>(&MdParamContainer::append))
        .def("append", static_cast<bool (MdParamContainer::*)(std::string, std::string, MdParamType, int)>(&MdParamContainer::append))
        .def("append", static_cast<bool (MdParamContainer::*)(std::string, std::string, MdParamType, float)>(&MdParamContainer::append))
        .def("append", static_cast<bool (MdParamContainer::*)(std::string, std::string, MdParamType, std::string)>(&MdParamContainer::append))
        .def("getParam", &MdParamContainer::getParam)
        .def("hasParam", &MdParamContainer::hasParam)
        .def("set", static_cast<bool (MdParamContainer::*)(std::string, bool, size_t)>(&MdParamContainer::set), arg("paramName"), arg("v"), arg("index") = 0)
        .def("set", static_cast<bool (MdParamContainer::*)(std::string, int, size_t)>(&MdParamContainer::set), arg("paramName"), arg("v"), arg("index") = 0)
        .def("set", static_cast<bool (MdParamContainer::*)(std::string, float, size_t)>(&MdParamContainer::set), arg("paramName"), arg("v"), arg("index") = 0)
        .def("set", static_cast<bool (MdParamContainer::*)(std::string, std::string, size_t)>(&MdParamContainer::set), arg("paramName"), arg("v"), arg("index") = 0)
        .def("getBool", &MdParamContainer::getBool, arg("paramName"), arg("index")=0)
        .def("getInt", &MdParamContainer::getInt, arg("paramName"), arg("index")=0)
        .def("getFloat", &MdParamContainer::getFloat, arg("paramName"), arg("index")=0)
        .def("getString", &MdParamContainer::getString, arg("paramName"), arg("index")=0)
        .def("getDefaultBool", &MdParamContainer::getDefaultBool)
        .def("getDefaultInt", &MdParamContainer::getDefaultInt)
        .def("getDefaultFloat", &MdParamContainer::getDefaultFloat)
        .def("getDefaultString", &MdParamContainer::getDefaultString);
}
