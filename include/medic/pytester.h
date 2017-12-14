#ifndef MEDIC_PYTESTER_INCLUDED
#define MEDIC_PYTESTER_INCLUDED


#include <Python.h>
#include <string>
#include "medic/py.h"
#include "medic/node.h"
#include "medic/report.h"
#include "medic/tester.h"


namespace MEDIC
{
    class MdPyTester : public MdTester
    {
        public:
            MdPyTester(PyObject *tester);
            ~MdPyTester();
            bool Match(const MdNode *node) const;
            // MdParamContainer *GetParameters() const;
            virtual MdReport *test(const MdNode *node) const;
            bool fix(const MdReport *report, MdParamContainer *params) const;
            static bool IsValidTester(PyObject *instance, PyObject *baseClass=NULL);

        private:
            PyObject *m_tester;
            PyObject *m_func_match;
            PyObject *m_func_get_parameters;
            PyObject *m_func_test;
            PyObject *m_func_fix;
    };
}


#endif // MEDIC_PYTESTER_INCLUDED