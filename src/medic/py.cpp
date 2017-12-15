#include "medic/py.h"


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
        }
        #endif // _DEBUG

        return NULL;
    }

    return func;
}
