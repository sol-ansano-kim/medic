#include "medic/node.h"


using namespace MEDIC;


MdNode::MdNode()
    : m_pyobj(NULL) {}

MdNode::~MdNode()
{
    if (m_pyobj != NULL)
    {
        Py_DECREF(m_pyobj);
    }
}

std::string MdNode::name() const
{
    return "UNKNOWN";
}

std::string MdNode::type() const
{
    return "UNKNOWN";
}

PyObject *MdNode::getPythonObject() const
{
    return m_pyobj;
}
