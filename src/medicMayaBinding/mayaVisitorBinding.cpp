#include "medicMayaBinding/mayaVisitorBinding.h"
#include <pybind11/stl.h>


using namespace pybind11;
using namespace MEDIC;


void bindMayaVisitor(pybind11::module mod)
{
    class_<MdMayaVisitor, std::unique_ptr<MdMayaVisitor, nodelete>>(mod, "MayaVisitor")
        .def(init<const MdPluginManager *>())
        .def("setKarte", static_cast<bool (MdMayaVisitor::*)(const MdKarte*)>(&MdMayaVisitor::setKarte))
        .def("setKarte", static_cast<bool (MdMayaVisitor::*)(const std::string&)>(&MdMayaVisitor::setKarte))
        .def("getTester", &MdMayaVisitor::getTester)
        .def("getTesters", &MdMayaVisitor::getTesters)
        .def("hasTester", static_cast<bool (MdMayaVisitor::*)(const std::string&)>(&MdMayaVisitor::hasTester))
        .def("hasTester", static_cast<bool (MdMayaVisitor::*)(const MdTester*)>(&MdMayaVisitor::hasTester))
        .def("test", static_cast<void (MdMayaVisitor::*)(const std::string &)>(&MdMayaVisitor::test))
        .def("test", static_cast<void (MdMayaVisitor::*)(const MdTester *)>(&MdMayaVisitor::test))
        .def("testAll", &MdMayaVisitor::testAll)
        .def("reset", &MdMayaVisitor::reset)
        .def("__collectNodes", &MdMayaVisitor::collectNodes);
}
