#include "medicBinding/visitorBinding.h"
#include <pybind11/stl.h>


using namespace pybind11;
using namespace MEDIC;


void bindVisitor(pybind11::module mod)
{
    class_<MdVisitor>(mod, "Visitor")
        .def(init<const MdPluginManager *>())
        .def("setKarte", static_cast<bool (MdVisitor::*)(const std::string&)>(&MdVisitor::setKarte))
        .def("setKarte", static_cast<bool (MdVisitor::*)(const MdKarte*)>(&MdVisitor::setKarte))
        .def("getTester", &MdVisitor::getTester)
        .def("getTesters", &MdVisitor::getTesters)
        .def("hasTester", static_cast<bool (MdVisitor::*)(const std::string&)>(&MdVisitor::hasTester))
        .def("hasTester", static_cast<bool (MdVisitor::*)(const MdTester*)>(&MdVisitor::hasTester))
        .def("test", static_cast<void (MdVisitor::*)(const std::string &)>(&MdVisitor::test))
        .def("test", static_cast<void (MdVisitor::*)(const MdTester *)>(&MdVisitor::test))
        .def("testAll", &MdVisitor::testAll)
        .def("reset", &MdVisitor::reset);
}
