#ifndef MEDIC_PY_INCLUDED
#define MEDIC_PY_INCLUDED


#include <Python.h>
#include <string>


namespace MEDIC
{
    static PyObject* PyBlankTuple = PyTuple_New(0);

    PyObject *getPyFunction(PyObject *instance, const char* funcName);
}


#endif // MEDIC_PY_INCLUDED