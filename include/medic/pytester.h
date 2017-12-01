#ifndef MEDIC_PYTESTER_INCLUDED
#define MEDIC_PYTESTER_INCLUDED


#include <string>
#include <Python.h>
#include "medic/py.h"


namespace MEDIC
{
    typedef MdPyContainer MdPyNode;
    typedef MdPyContainer MdPyReport;
    typedef MdPyContainer MdPyParamContainer;

    class MdPyTester
    {
        public:
            MdPyTester(PyObject *tester);
            ~MdPyTester();
            const std::string &Name() const;
            const std::string &Description() const;
            bool Match(MdPyNode *node) const;
            bool IsFixable() const;
            MdPyParamContainer *GetParameters() const;
            MdPyReport *test(MdPyNode *node) const;
            bool fix(MdPyReport *report, MdPyParamContainer *params) const;
            static bool IsValidTester(PyObject *instance, PyObject *baseClass=NULL);

        private:
            bool m_isfixable;
            std::string m_name;
            std::string m_description;
            PyObject *m_tester;
            PyObject *m_func_match;
            PyObject *m_func_get_parameters;
            PyObject *m_func_test;
            PyObject *m_func_fix;
    };
}


#endif // MEDIC_PYTESTER_INCLUDED