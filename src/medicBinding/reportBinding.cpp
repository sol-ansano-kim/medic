#include "medicBinding/reportBinding.h"


using namespace pybind11;
using namespace MEDIC;


void bindReport(pybind11::module mod)
{
    class_<MdReport, std::unique_ptr<MdReport, nodelete>>(mod, "Report")
        .def(init<const MdNode *>())
        .def("node", &MdReport::node);
}
