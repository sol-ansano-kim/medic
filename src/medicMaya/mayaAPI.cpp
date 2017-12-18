#include "medic/py.h"
#include "medicMaya/mayaAPI.h"
#include <maya/MFn.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>


using namespace MEDIC;


MdPyMayaSelectionList *MdPyMayaSelectionList::Instance()
{
    PyObject *pyapi;
    PyObject *sel_class;
    PyObject *selection;
    PyObject *add;
    PyObject *clear;
    PyObject *get_dep;
    PyObject *mobj;
    PyObject *mfn_dg;
    PyObject *mfn_dag;

    if (!Py_IsInitialized())
    {
        error("Python is not initialized");

        return NULL;
    }

    pyapi = PyImport_ImportModule("maya.OpenMaya");
    if (pyapi == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        error("Failed to load module 'maya.OpenMaya'");

        return NULL;
    }

    Py_INCREF(pyapi);

    mobj = getPyFunction(pyapi, "MObject");
    if (mobj == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        error("Cannot find 'MObject'");

        Py_DECREF(pyapi);

        return NULL;
    }

    mfn_dg = getPyFunction(pyapi, "MFnDependencyNode");
    if (mfn_dg == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        error("Cannot find 'MFnDependencyNode'");

        Py_DECREF(pyapi);

        return NULL;
    }

    mfn_dag = getPyFunction(pyapi, "MFnDagNode");
    if (mfn_dag == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        error("Cannot find 'MFnDagNode'");

        Py_DECREF(pyapi);

        return NULL;
    }

    sel_class = getPyFunction(pyapi, "MSelectionList");
    if (sel_class == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        error("Cannot find 'MSelectionList'");

        Py_DECREF(pyapi);

        return NULL;
    }

    Py_INCREF(sel_class);

    selection = PyObject_CallObject(sel_class, PyBlankTuple);
    if (selection == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        error("Failed to create 'MSelectionList'");
        Py_DECREF(pyapi);
        Py_DECREF(sel_class);

        return NULL;
    }

    Py_DECREF(sel_class);
    Py_DECREF(pyapi);

    Py_INCREF(selection);

    add = getPyFunction(selection, "add");
    if (add == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        error("Cannot find 'MSelectionList.add'");
        Py_DECREF(selection);

        return NULL;
    }

    Py_INCREF(add);

    clear = getPyFunction(selection, "clear");
    if (clear == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        error("Cannot find 'MSelectionList.clear'");
        Py_DECREF(add);
        Py_DECREF(selection);

        return NULL;
    }

    Py_INCREF(clear);

    get_dep = getPyFunction(selection, "getDependNode");
    if (get_dep == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        error("Cannot find 'MSelectionList.getDependNode'");
        Py_DECREF(add);
        Py_INCREF(clear);
        Py_DECREF(selection);

        return NULL;
    }

    Py_INCREF(get_dep);

    MdPyMayaSelectionList *pysel = new MdPyMayaSelectionList();
    pysel->m_sel = selection;
    pysel->m_func_add = add;
    pysel->m_func_clear = clear;
    pysel->m_func_get_dep = get_dep;
    pysel->m_class_mfn_dep = mfn_dg;
    pysel->m_class_mfn_dag = mfn_dag;
    pysel->m_class_object = mobj;

    return pysel;
}

PyObject *MdPyMayaSelectionList::getObject(std::string name)
{
    PyObject *arg1;
    PyObject *arg2;
    PyObject *res;
    PyObject *arg_name;
    PyObject *arg_index;

    PyObject_CallObject(m_func_clear, PyBlankTuple);
    if (PyErr_Occurred())
    {
        #ifdef _DEBUG
        PyErr_Print();
        #endif // _DEBUG

        return NULL;
    }

    arg1 = PyTuple_New(1);
    arg_name = PyString_FromString(name.c_str());
    PyTuple_SetItem(arg1, 0, arg_name);
    Py_INCREF(arg1);
    Py_INCREF(arg_name);

    PyObject_CallObject(m_func_add, arg1);
    if (PyErr_Occurred())
    {
        #ifdef _DEBUG
        PyErr_Print();
        #endif // _DEBUG

        Py_DECREF(arg1);
        Py_DECREF(arg_name);

        return NULL;
    }

    Py_DECREF(arg1);
    Py_DECREF(arg_name);

    res = PyObject_CallObject(m_class_object, PyBlankTuple);
    if (res == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        error("Failed to create MObject");

        return NULL;
    }

    arg2 = PyTuple_New(2);
    arg_index = PyInt_FromLong(0);
    PyTuple_SetItem(arg2, 0, arg_index);
    PyTuple_SetItem(arg2, 1, res);

    Py_INCREF(arg2);
    Py_INCREF(res);
    Py_INCREF(arg_index);

    PyObject_CallObject(m_func_get_dep, arg2);

    Py_DECREF(arg2);
    Py_DECREF(arg_index);

    if (PyErr_Occurred())
    {
        #ifdef _DEBUG
        PyErr_Print();
        #endif // _DEBUG

        Py_DECREF(res);
        return NULL;
    }    

    return res;
}

PyObject *MdPyMayaSelectionList::getDg(PyObject *mobject)
{
    PyObject *res;
    PyObject *args;

    args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, mobject);
    Py_INCREF(args);

    res = PyObject_CallObject(m_class_mfn_dep, args);
    Py_DECREF(args);

    if (res == NULL || PyErr_Occurred())
    {
        #ifdef _DEBUG
        PyErr_Print();
        #endif // _DEBUG

        return NULL;
    }

    Py_INCREF(res);

    return res;
}

PyObject *MdPyMayaSelectionList::getDag(PyObject *mobject)
{
    PyObject *res;
    PyObject *args;

    args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, mobject);
    Py_INCREF(args);

    res = PyObject_CallObject(m_class_mfn_dag, args);
    Py_DECREF(args);

    if (res == NULL || PyErr_Occurred())
    {
        #ifdef _DEBUG
        PyErr_Print();
        #endif // _DEBUG

        return NULL;
    }

    Py_INCREF(res);

    return res;
}

PyObject *MdPyMayaSelectionList::getBlankDag()
{
    PyObject *res;

    res = PyObject_CallObject(m_class_mfn_dag, PyBlankTuple);

    if (res == NULL || PyErr_Occurred())
    {
        #ifdef _DEBUG
        PyErr_Print();
        #endif // _DEBUG

        return NULL;
    }

    Py_INCREF(res);

    return res;
}

PyObject *MdPyMayaSelectionList::getDagPath(PyObject *mdagnode)
{
    PyObject *dagpath = NULL;
    PyObject *fn_getpath;
    PyObject *args;

    fn_getpath = getPyFunction(mdagnode, "getPath");
    if (fn_getpath == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        return NULL;
    }

    Py_INCREF(fn_getpath);

    args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, dagpath);

    Py_INCREF(args);
    PyObject_CallObject(fn_getpath, args);
    Py_DECREF(args);

    if (PyErr_Occurred())
    {
        #ifdef _DEBUG
        PyErr_Print();
        #endif // _DEBUG

        Py_DECREF(fn_getpath);
        return NULL;
    }

    Py_DECREF(fn_getpath);

    Py_INCREF(dagpath);

    return dagpath;
}

MdPyMayaSelectionList::MdPyMayaSelectionList() {}

MdPyMayaSelectionList::~MdPyMayaSelectionList()
{
    if (m_sel != NULL)
    {
        Py_DECREF(m_sel);
    }

    if (m_func_add != NULL)
    {
        Py_DECREF(m_func_add);
    }

    if (m_func_clear != NULL)
    {
        Py_DECREF(m_func_clear);
    }

    if (m_func_get_dep != NULL)
    {
        Py_DECREF(m_func_get_dep);
    }
}


MdPyMayaSelectionList *PySelection = MdPyMayaSelectionList::Instance();


void MEDIC::GetMayaObject(std::string name, MObject &object)
{
    Selection.clear();
    Selection.add(name.c_str());

    Selection.getDependNode(0, object);
}


PyObject *MEDIC::GetPythonMayaObject(std::string name)
{
    MdPyMayaSelectionList *inst = MdPyMayaSelectionList::Instance();
    if (inst == NULL)
    {
        return NULL;
    }

    return inst->getObject(name);
}


PyObject *MEDIC::GetPythonMayaDg(PyObject *mobject)
{
    if (mobject == NULL)
    {
        return NULL;
    }

    MdPyMayaSelectionList *inst = MdPyMayaSelectionList::Instance();
    if (inst == NULL)
    {
        return NULL;
    }

    return inst->getDg(mobject);
}


PyObject *MEDIC::GetPythonMayaDag(PyObject *mobject)
{
    if (mobject == NULL)
    {
        return NULL;
    }

    MdPyMayaSelectionList *inst = MdPyMayaSelectionList::Instance();
    if (inst == NULL)
    {
        return NULL;
    }

    return inst->getDag(mobject);
}


PyObject *MEDIC::GetPythonMayaBlankDag()
{
    MdPyMayaSelectionList *inst = MdPyMayaSelectionList::Instance();
    if (inst == NULL)
    {
        return NULL;
    }

    return inst->getBlankDag();
}


PyObject *MEDIC::GetPythonMayaDagPath(PyObject *mdagnode)
{
    if (mdagnode == NULL)
    {
        return NULL;
    }

    MdPyMayaSelectionList *inst = MdPyMayaSelectionList::Instance();
    if (inst == NULL)
    {
        return NULL;
    }

    return inst->getDagPath(mdagnode);
}


bool MEDIC::IsValidObject(MObject &obj)
{
    return (!obj.isNull() && obj.hasFn(MFn::kDependencyNode)) ? true : false;
}


MObjectArray MEDIC::CollectNodes()
{
    MObjectArray obj_array;

    MItDependencyNodes it;
    while (!it.isDone())
    {
        MObject obj = it.thisNode();
        if (MEDIC::IsValidObject(obj))
        {
            MFnDependencyNode dn(obj);
            if (!dn.isShared() && !dn.isDefaultNode() && dn.canBeWritten())
            {
                obj_array.append(obj);
            }
        }
        
        it.next();
    }

    return obj_array;
}
