#include "medic/logger.h"
#include "medic/pytester.h"


using namespace MEDIC;


MdPyTester::MdPyTester(PyObject *tester)
    : m_tester(tester)
{
    m_func_match = getPyFunction(tester, "Match");
    m_func_get_parameters = getPyFunction(tester, "GetParameters");
    m_func_test = getPyFunction(tester, "test");
    m_func_fix = getPyFunction(tester, "fix");

    PyObject *res;
    PyObject *func;

    func = getPyFunction(tester, "Name");
    res = PyObject_CallObject(func, PyBlankTuple);
    m_name = (std::string)PyString_AsString(res);
    Py_DECREF(func);
    Py_DECREF(res);

    func = getPyFunction(tester, "Description");
    res = PyObject_CallObject(func, PyBlankTuple);
    m_description = (std::string)PyString_AsString(res);
    Py_DECREF(func);
    Py_DECREF(res);

    func = getPyFunction(tester, "IsFixable");
    res = PyObject_CallObject(func, PyBlankTuple);
    m_isfixable = (PyObject_IsTrue(res)) ? true : false;
    Py_DECREF(func);
    Py_DECREF(res);
}

MdPyTester::~MdPyTester()
{
    Py_DECREF(m_func_match);
    Py_DECREF(m_func_get_parameters);
    Py_DECREF(m_func_test);
    Py_DECREF(m_func_fix);
    Py_DECREF(m_tester);
}

bool MdPyTester::Match(const MdNode *node) const
{
    bool result = false;
    PyObject *res;
    PyObject *arg;
    PyObject *py_node;

    py_node = node->getPythonObject();
    if (py_node == NULL)
    {
        warn("Failed to get python object");

        return false;
    }

    arg = PyTuple_New(1);
    PyTuple_SetItem(arg, 0, py_node);

    res = PyObject_CallObject(m_func_match, arg);
    Py_DECREF(arg);

    if (res == NULL)
    {
        #ifdef _DEBUG
        if (!PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        return false;
    }

    result = (PyObject_IsTrue(res)) ? true : false;

    Py_DECREF(res);

    return result;
}

// MdParamContainer *MdPyTester::GetParameters() const
// {
//     PyObject *param = PyObject_CallObject(m_func_get_parameters, PyBlankTuple);

//     if (param == NULL)
//     {
//         #ifdef _DEBUG
//         if (PyErr_Occurred())
//         {
//             PyErr_Print();
//         }
//         #endif // _DEBUG

//         return NULL;
//     }

//     return new MdParamContainer(param);
// }

MdReport *MdPyTester::test(const MdNode *node) const
{
    MdReport *report = NULL;
    PyObject *res;
    PyObject *arg;
    PyObject *py_node;

    py_node = node->getPythonObject();
    if (py_node == NULL)
    {
        warn("Failed to get python object.");

        return NULL;
    }

    arg = PyTuple_New(1);
    PyTuple_SetItem(arg, 0, py_node);

    res = PyObject_CallObject(m_func_test, arg);
    Py_DECREF(arg);

    if (res == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        return NULL;
    }

    if (res != Py_None)
    {
        report = new MdReport(node);
    }

    Py_DECREF(res);

    return report;
}

bool MdPyTester::fix(const MdReport *report, MdParamContainer *params) const
{
    PyObject *res;
    PyObject *arg;
    PyObject *py_node;
    bool result = false;

    if (!m_isfixable)
    {
        return false;
    }

    py_node = report->node()->getPythonObject();
    if (py_node == NULL)
    {
        warn("Failed to get python object");

        return false;
    }

    arg = PyTuple_New(2);
    PyTuple_SetItem(arg, 0, py_node);
    // TODO : set a correct python MdParamContainer
    PyTuple_SetItem(arg, 1, Py_None);

    res = PyObject_CallObject(m_func_fix, arg);
    Py_DECREF(arg);

    if (res == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG

        return false;
    }

    result = (PyObject_IsTrue(res)) ? true : false;

    Py_DECREF(res);

    return result;
}

bool MdPyTester::IsValidTester(PyObject *instance, PyObject *baseClass)
{
    PyObject *func;
    PyObject *res;

    if (instance == NULL)
    {
        error("NULL object");

        return false;
    }

    if (baseClass != NULL && PyObject_IsInstance(instance, baseClass) != 1)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif //_DEBUG
        warn("Given object is not a instance of base class");

        return false;
    }

    func = getPyFunction(instance, "Name");
    if (func == NULL)
    {
        return false;
    }

    res = PyObject_CallObject(func, PyBlankTuple);
    Py_DECREF(func);

    if (res == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG
        warn("Invalid function : Name()");

        return false;
    }
    if (!PyObject_TypeCheck(res, &PyBaseString_Type))
    {
        warn("Invalid return variable type : Name()");

        Py_DECREF(res);
        return false;
    }

    Py_DECREF(res);

    func = getPyFunction(instance, "Description");
    if (func == NULL)
    {
        return false;
    }

    res = PyObject_CallObject(func, PyBlankTuple);
    Py_DECREF(func);

    if (res == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG
        warn("Invalid function : Description()");

        return false;
    }
    if (!PyObject_TypeCheck(res, &PyBaseString_Type))
    {
        warn("Invalid return variable type : Description()");

        Py_DECREF(res);
        return false;
    }

    Py_DECREF(res);

    func = getPyFunction(instance, "Match");
    if (func == NULL)
    {
        return false;
    }

    Py_DECREF(func);

    func = getPyFunction(instance, "IsFixable");
    if (func == NULL)
    {
        return false;
    }

    res = PyObject_CallObject(func, PyBlankTuple);
    Py_DECREF(func);

    if (res == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }
        #endif // _DEBUG
        warn("Invalid function : IsFixable()");

        return false;
    }
    if (!PyObject_TypeCheck(res, &PyBool_Type))
    {
        warn("Invalid return variable type : IsFixable()");

        Py_DECREF(res);
        return false;
    }

    Py_DECREF(res);

    func = getPyFunction(instance, "GetParameters");
    if (func == NULL)
    {
        return false;
    }

    Py_DECREF(func);

    func = getPyFunction(instance, "test");
    if (func == NULL)
    {
        return false;
    }

    Py_DECREF(func);

    func = getPyFunction(instance, "fix");
    if (func == NULL)
    {
        return false;
    }

    Py_DECREF(func);

    return true;
}