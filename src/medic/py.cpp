#include "medic/py.h"
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG


using namespace MEDIC;


PyObject *MEDIC::getPyFunction(PyObject *instance, const char* funcName)
{
    PyObject *func = PyObject_GetAttrString(instance, funcName);
    if (func == NULL || !PyCallable_Check(func))
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
            std::cout << "[MEDIC] ERROR - Failed to function check\n";
        }
        #endif // _DEBUG

        return NULL;
    }

    return func;
}



MdPyContainer::MdPyContainer(PyObject *obj)
    : m_object(obj)
{
    Py_INCREF(m_object);
}

MdPyContainer::MdPyContainer(const MdPyContainer* pycon)
    : m_object(pycon->m_object)
{
    Py_INCREF(m_object);
}

MdPyContainer::~MdPyContainer()
{
    Py_DECREF(m_object);
}

PyObject *MdPyContainer::object()
{
    return m_object;
}
