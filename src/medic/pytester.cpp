#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG
#include "medic/pytester.h"


using namespace MEDIC;


MdPyTester::MdPyTester(PyObject *tester)
    : m_tester(tester)
{
    Py_INCREF(m_tester);

    m_func_match = getPyFunction(tester, "Match");
    m_func_get_parameters = getPyFunction(tester, "GetParameters");
    m_func_test = getPyFunction(tester, "test");
    m_func_fix = getPyFunction(tester, "fix");

    Py_INCREF(m_func_match);
    Py_INCREF(m_func_get_parameters);
    Py_INCREF(m_func_test);
    Py_INCREF(m_func_fix);

    PyObject *res;
    PyObject *func;

    func = getPyFunction(tester, "Name");
    res = PyObject_CallObject(func, PyBlankTuple);
    m_name = (std::string)PyString_AsString(res);
    Py_DECREF(res);

    func = getPyFunction(tester, "Description");
    res = PyObject_CallObject(func, PyBlankTuple);
    m_description = (std::string)PyString_AsString(res);
    Py_DECREF(res);

    func = getPyFunction(tester, "IsFixable");
    res = PyObject_CallObject(func, PyBlankTuple);
    m_isfixable = (PyObject_IsTrue(res)) ? true : false;
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

bool MdPyTester::Match(MdNode *node) const
{
    bool result = false;
    PyObject *res;
    PyObject *arg;
    PyObject *py_node;

    py_node = node->getPythonObject();
    if (py_node == NULL)
    {
        #ifdef _DEBUG
        std::cout << "[MEDIC] WARNING - Failed to get python object.\n";
        #endif // _DEBUG

        return false;
    }

    Py_INCREF(py_node);

    arg = PyTuple_New(1);
    PyTuple_SetItem(arg, 0, py_node);

    res = PyObject_CallObject(m_func_match, arg);
    Py_DECREF(arg);
    Py_DECREF(py_node);

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

MdReport *MdPyTester::test(MdNode *node) const
{
    MdReport *report = NULL;
    PyObject *res;
    PyObject *arg;
    PyObject *py_node;

    py_node = node->getPythonObject();
    if (py_node == NULL)
    {
        #ifdef _DEBUG
        std::cout << "[MEDIC] WARNING - Failed to get python object.\n";
        #endif // _DEBUG

        return NULL;
    }

    Py_INCREF(py_node);

    arg = PyTuple_New(1);
    PyTuple_SetItem(arg, 0, py_node);

    res = PyObject_CallObject(m_func_test, arg);
    Py_DECREF(arg);
    Py_DECREF(py_node);

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

bool MdPyTester::fix(MdReport *report, MdParamContainer *params) const
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
        #ifdef _DEBUG
        std::cout << "[MEDIC] WARNING - Failed to get python object.\n";
        #endif // _DEBUG

        return false;
    }

    Py_INCREF(py_node);

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
        #ifdef _DEBUG
        std::cout << "[MEDIC] ERROR - NULL object\n";
        #endif // _DEBUG

        return false;
    }

    if (baseClass != NULL && PyObject_IsInstance(instance, baseClass) != 1)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        std::cout << "[MEDIC] ERROR - Given object is not a instance of base class\n";
        #endif //_DEBUG

        return false;
    }

    func = getPyFunction(instance, "Name");
    if (func == NULL)
    {
        return false;
    }
    res = PyObject_CallObject(func, PyBlankTuple);
    if (res == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
            std::cout << "[MEDIC] ERROR - Invalid function : Name()\n";
        }
        #endif // _DEBUG

        return false;
    }
    if (!PyObject_TypeCheck(res, &PyBaseString_Type))
    {
        #ifdef _DEBUG
        std::cout << "[MEDIC] WARNING - Invalid return variable type : Name()\n";
        #endif // _DEBUG

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
    if (res == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
            std::cout << "[MEDIC] ERROR - Invalid function : Description()\n";
        }
        #endif // _DEBUG

        return false;
    }
    if (!PyObject_TypeCheck(res, &PyBaseString_Type))
    {
        #ifdef _DEBUG
        std::cout << "[MEDIC] WARNING - Invalid return variable type : Description()\n";
        #endif // _DEBUG

        Py_DECREF(res);
        return false;
    }

    Py_DECREF(res);

    func = getPyFunction(instance, "Match");
    if (func == NULL)
    {
        return false;
    }

    func = getPyFunction(instance, "IsFixable");
    if (func == NULL)
    {
        return false;
    }
    res = PyObject_CallObject(func, PyBlankTuple);
    if (res == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
            std::cout << "[MEDIC] ERROR - Invalid function : IsFixable()\n";
        }
        #endif // _DEBUG

        return false;
    }
    if (!PyObject_TypeCheck(res, &PyBool_Type))
    {
        #ifdef _DEBUG
        std::cout << "[MEDIC] WARNING - Invalid return variable type : IsFixable()\n";
        #endif // _DEBUG

        Py_DECREF(res);
        return false;
    }

    Py_DECREF(res);

    func = getPyFunction(instance, "GetParameters");
    if (func == NULL)
    {
        return false;
    }

    func = getPyFunction(instance, "test");
    if (func == NULL)
    {
        return false;
    }

    func = getPyFunction(instance, "fix");
    if (func == NULL)
    {
        return false;
    }

    return true;
}