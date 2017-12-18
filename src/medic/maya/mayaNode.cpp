#include <Python.h>
#include "medic/maya/mayaNode.h"
#include "medic/maya/mayaAPI.h"


using namespace MEDIC;


MdMayaNode::MdMayaNode()
    : MdNode(), m_is_dag(false) {}

MdMayaNode::MdMayaNode(MObject &obj)
    : MdNode()
{
    m_obj = obj;
    m_dg.setObject(obj);
    m_is_dag = false;

    if (obj.hasFn(MFn::kDagNode))
    {
        m_is_dag = true;
        m_dag.setObject(obj);
    }
}

MdMayaNode::MdMayaNode(std::string name)
      : MdNode()
{
    GetMayaObject(name, m_obj);
    m_dg.setObject(m_obj);
    m_is_dag = false;

    if (m_obj.hasFn(MFn::kDagNode))
    {
        m_is_dag = true;
        m_dag.setObject(m_obj);
    }
}

MdMayaNode::~MdMayaNode()
{
    if (m_py_obj != NULL)
    {
        Py_DECREF(m_py_obj);
    }

    if (m_py_dg != NULL)
    {
        Py_DECREF(m_py_dg);
    }

    if (m_py_dag != NULL)
    {
        Py_DECREF(m_py_dag);
    }

    if (m_py_path != NULL)
    {
        Py_DECREF(m_py_path);
    }
}

std::string MdMayaNode::name() const
{
    if (!isDag())
    {
        return (std::string)m_dg.name().asChar();
    }

    return (std::string)m_dag.partialPathName().asChar();
}

std::string MdMayaNode::type() const
{
    return (std::string)m_dg.typeName().asChar();
}

bool MdMayaNode::isDag() const
{
    return m_is_dag;
}

MDagPath MdMayaNode::getPath()
{
    if (!isDag())
    {
        return MDagPath();
    }

    MDagPath path;
    m_dag.getPath(path);

    return path;
}

MObject &MdMayaNode::object()
{
    return m_obj;
}

MFnDependencyNode &MdMayaNode::dg()
{
    return m_dg;
}

MFnDagNode &MdMayaNode::dag()
{
    return m_dag;
}


PyObject *MdMayaNode::pyGetPath()
{
    if (m_py_path == NULL)
    {
        if (!isDag())
        {
            m_py_path = GetPythonMayaBlankDag();
        }
        else
        {
            m_py_path = pyObject();
        }
    }

    return m_py_path;
}

PyObject *MdMayaNode::pyObject()
{

    if (m_py_obj == NULL)
    {
        m_py_obj = GetPythonMayaObject(name());
    }

    return m_py_obj;
}

PyObject *MdMayaNode::pyDg()
{
    if (m_py_dg == NULL)
    {
        m_py_dg = GetPythonMayaDg(pyObject());
    }

    return m_py_dg;
}

PyObject *MdMayaNode::pyDag()
{
    if (m_py_dag == NULL)
    {
        m_py_dag = GetPythonMayaDag(pyObject());
    }

    return m_py_dag;
}

