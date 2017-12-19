#include "medicMayaBinding/mayaNodeBinding.h"


using namespace pybind11;
using namespace MEDIC;


handle getHandle(PyObject *obj)
{
    return (obj == NULL) ? handle(Py_None) : handle(obj);
}

void bindMayaNode(pybind11::module mod)
{
    class_<MdMayaNode, std::unique_ptr<MdMayaNode, nodelete>>(mod, "MayaNode")
        .def(init())
        .def(init<std::string>())
        .def("name", &MdMayaNode::name)
        .def("type", &MdMayaNode::type)
        .def("isDag", &MdMayaNode::isDag)
        .def("path", [](MdMayaNode &self) { return getHandle(self.pyPath()); })
        .def("object", [](MdMayaNode &self) { return getHandle(self.pyObject()); })
        .def("dg", [](MdMayaNode &self) { return getHandle(self.pyDg()); })
        .def("dag", [](MdMayaNode &self) { return getHandle(self.pyDag()); });
}
