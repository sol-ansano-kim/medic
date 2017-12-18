#include "medic/py/nodeBind.h"


using namespace pybind11;
using namespace MEDIC;


void bindNode(pybind11::module mod)
{
    class_<MdNode, std::unique_ptr<MdNode, nodelete>>(mod, "Node")
        .def(init())
        .def("name", &MdNode::name)
        .def("type", &MdNode::type);
}
