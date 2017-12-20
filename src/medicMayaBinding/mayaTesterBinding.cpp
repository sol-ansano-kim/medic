#include "medicMayaBinding/mayaTesterBinding.h"


using namespace pybind11;
using namespace MEDIC;


void bindMayaTester(pybind11::module mod)
{
    class_<MdMayaTester, std::unique_ptr<MdMayaTester, nodelete>>(mod, "MayaTester")
        .def(init())
        .def("Name", &MdMayaTester::Name)
        .def("Description", &MdMayaTester::Description)
        .def("IsFixable", &MdMayaTester::IsFixable)
        .def("GetParameters", &MdMayaTester::GetParameters)
        .def("Match", static_cast<bool (MdMayaTester::*)(const MdMayaNode *) const>(&MdMayaTester::Match))
        .def("test", static_cast<MdMayaReport *(MdMayaTester::*)(const MdMayaNode *) const>(&MdMayaTester::test))
        .def("fix", static_cast<bool (MdMayaTester::*)(const MdMayaReport *report, MdParamContainer *params) const>(&MdMayaTester::fix));
}
