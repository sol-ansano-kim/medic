#ifndef MEDIC_PYTESTER_INCLUDED
#define MEDIC_PYTESTER_INCLUDED


#include "medic/tester.h"
#include <Python.h>


namespace MEDIC
{
    class MdPyTester
    {
        public:
            MdPyTester(PyObject *tester);
            virtual ~MdPyTester();
            static bool IsValidTester(PyObject *instance, PyObject *baseClass=NULL);
            // virtual const std::string &Name() const;
            // virtual const std::string &Description() const;
            // virtual bool Match(MdNode *node) const;
            // virtual bool IsFixable() const;
            // virtual MdParamContainer *GetParameters() const;
            // virtual MdReport *test(const MdNode *node) const;
            // virtual bool fix(MdReport *report, MdParamContainer *params) const;

        private:
            static bool hasFunction(PyObject *instance, const char* funcName);

        private:
            PyObject *m_tester;
            static const std::string m_unknown;
    };
}


#endif