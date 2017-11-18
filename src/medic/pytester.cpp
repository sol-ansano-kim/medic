#ifdef _DEBUG
#include <iostream>
#endif
#include "medic/pytester.h"


using namespace MEDIC;


MdPyTester::MdPyTester(PyObject *tester)
    : m_tester(tester) {}

MdPyTester::~MdPyTester() {}


bool MdPyTester::IsValidTester(PyObject *instance, PyObject *baseClass)
{
    if (instance == NULL)
    {
        #ifdef _DEBUG
        std::cout << "[MEDIC] ERROR - NULL object\n";
        #endif

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

    if (!hasFunction(instance, "Name"))
    {
        return false;
    }
    if (!hasFunction(instance, "Description"))
    {
        return false;
    }
    if (!hasFunction(instance, "Match"))
    {
        return false;
    }
    if (!hasFunction(instance, "IsFixable"))
    {
        return false;
    }
    if (!hasFunction(instance, "GetParameters"))
    {
        return false;
    }
    if (!hasFunction(instance, "test"))
    {
        return false;
    }
    if (!hasFunction(instance, "fix"))
    {
        return false;
    }

    return true;
}

bool MdPyTester::hasFunction(PyObject *instance, const char* funcName)
{
    PyObject *func = PyObject_GetAttrString(instance, funcName);
    if (func == NULL || !PyCallable_Check(func))
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            std::cout << "[MEDIC] ERROR - Failed to function check\n";
            PyErr_Print();
        }
        #endif // _DEBUG

        return false;
    }

    return true;
}