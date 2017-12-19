#include <Python.h>
#include "medicMaya/mayaNode.h"
#include "medicMaya/mayaAPI.h"


using namespace MEDIC;


MdMayaNode::MdMayaNode()
    : MdNode(), m_is_dag(false), m_py_obj(NULL), m_py_dg(NULL), m_py_dag(NULL), m_py_path(NULL) {}

MdMayaNode::MdMayaNode(MObject &obj)
    : MdNode(), m_is_dag(false), m_py_obj(NULL), m_py_dg(NULL), m_py_dag(NULL), m_py_path(NULL)
{
    m_obj = obj;
    m_dg.setObject(obj);

    if (obj.hasFn(MFn::kDagNode))
    {
        m_is_dag = true;
        m_dag.setObject(obj);
    }
}

MdMayaNode::MdMayaNode(std::string name)
      : MdNode(), m_is_dag(false), m_py_obj(NULL), m_py_dg(NULL), m_py_dag(NULL), m_py_path(NULL)
{
    GetMayaObject(name, m_obj);
    m_dg.setObject(m_obj);

    if (m_obj.hasFn(MFn::kDagNode))
    {
        m_is_dag = true;
        m_dag.setObject(m_obj);
    }
}

MdMayaNode::~MdMayaNode()
{
    clearPyObjects();
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

MDagPath MdMayaNode::path()
{
    if (!isDag())
    {
        return MDagPath();
    }

    MDagPath path;
    m_dag.getPath(path);

    return path;
}

PyObject *MdMayaNode::pyObject()
{
    if (m_py_obj != NULL)
    {
        return m_py_obj;
    }

    if (!IsValidObject(m_obj))
    {
        m_py_obj = GetPythonMayaBlankObject();
    }
    else
    {
        m_py_obj = GetPythonMayaObject(name());
    }

    return m_py_obj;
}

PyObject *MdMayaNode::pyDg()
{
    if (m_py_dg != NULL)
    {
        return m_py_dg;
    }

    if (!IsValidObject(m_obj))
    {
        m_py_dg = GetPythonMayaBlankDg();
    }
    else
    {
        m_py_dg = GetPythonMayaDg(pyObject());
    }

    return m_py_dg;
}

PyObject *MdMayaNode::pyDag()
{
    if (m_py_dag != NULL)
    {
        return m_py_dag;
    }

    if (!IsValidObject(m_obj))
    {
        m_py_dag = GetPythonMayaBlankDag();
    }
    else
    {
        if (!isDag())
        {
            m_py_dag = GetPythonMayaBlankDag();
        }
        else
        {
            m_py_dag = GetPythonMayaDag(pyObject());
        }
    }

    return m_py_dag;
}

PyObject *MdMayaNode::pyPath()
{
    if (m_py_path != NULL)
    {
        return m_py_path;
    }

    if (!IsValidObject(m_obj))
    {
        m_py_path =  GetPythonMayaBlankDagPath();
    }
    else
    {
        if (!isDag())
        {
            m_py_path = GetPythonMayaBlankDagPath();
        }
        else
        {
            m_py_path = GetPythonMayaDagPath(pyDag());
        }
    }

    return m_py_path;
}

void MdMayaNode::clearPyObjects()
{
    if (m_py_obj != NULL)
    {
        Py_DECREF(m_py_obj);
        m_py_obj = NULL;
    }

    if (m_py_dg != NULL)
    {
        Py_DECREF(m_py_dg);
        m_py_dg = NULL;
    }

    if (m_py_dag != NULL)
    {
        Py_DECREF(m_py_dag);
        m_py_dag = NULL;
    }

    if (m_py_path != NULL)
    {
        Py_DECREF(m_py_path);
        m_py_path = NULL;
    }
}

