#include "medicMayaBinding/mayaReportBinding.h"


using namespace pybind11;
using namespace MEDIC;


void bindMayaReport(pybind11::module mod)
{
    class_<MdMayaReport, std::unique_ptr<MdMayaReport, nodelete>>(mod, "MayaReport")
        .def(init<MdMayaNode *>())
        .def(init<MdMayaNode *, MObject &>())
        .def("components", &MdMayaReport::components)
        .def("hasComponents", &MdMayaReport::hasComponents);
}
