#include "medicBinding/testerBinding.h"


using namespace pybind11;
using namespace MEDIC;


void bindTester(pybind11::module mod)
{
    class_<MdTester, std::unique_ptr<MdTester, nodelete>>(mod, "Tester")
        .def(init())
        .def("Name", &MdTester::Name)
        .def("Description", &MdTester::Description)
        .def("Match", &MdTester::Match)
        .def("IsFixable", &MdTester::IsFixable)
        .def("GetParameters", &MdTester::GetParameters)
        .def("test", &MdTester::test)
        .def("fix", &MdTester::fix);
}
