#ifndef MEDIC_PY_INCLUDED
#define MEDIC_PY_INCLUDED


#include <Python.h>
#include <string>

namespace MEDIC
{
    static PyObject* PyBlankTuple = PyTuple_New(0);

    PyObject *getPyFunction(PyObject *instance, const char* funcName);

    class MdPyContainer
    {
        public:
            MdPyContainer(PyObject *obj);
            MdPyContainer(const MdPyContainer* pycon);
            ~MdPyContainer();
            PyObject *object();

        private:
            PyObject *m_object;
    };
}


#endif // MEDIC_PY_INCLUDED