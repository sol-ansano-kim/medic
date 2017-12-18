#ifndef MEDIC_MAYA_MAYAAPI_INCLUDED
#define MEDIC_MAYA_MAYAAPI_INCLUDED


#include <Python.h>
#include <string>
#include <maya/MString.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MSelectionList.h>
#include "medic/logger.h"


namespace MEDIC
{
    class MdPyMayaSelectionList
    {
        public:
            static MdPyMayaSelectionList *Instance();
            PyObject *getObject(std::string name);
            PyObject *getDg(PyObject *mobject);
            PyObject *getDag(PyObject *mobject);
            PyObject *getBlankDag();
            PyObject *getDagPath(PyObject *mdagnode);

        private:
            MdPyMayaSelectionList();
            ~MdPyMayaSelectionList();

            PyObject *m_sel;
            PyObject *m_func_add;
            PyObject *m_func_clear;
            PyObject *m_func_get_dep;
            PyObject *m_class_object;
            PyObject *m_class_mfn_dep;
            PyObject *m_class_mfn_dag;
    };

    static MSelectionList Selection;
    static MdPyMayaSelectionList *PySelection;

    void GetMayaObject(std::string name, MObject &object);
    PyObject *GetPythonMayaObject(std::string name);
    PyObject *GetPythonMayaDg(PyObject *mobject);
    PyObject *GetPythonMayaDag(PyObject *mobject);
    PyObject *GetPythonMayaBlankDag();
    PyObject *GetPythonMayaDagPath(PyObject *mdagnode);
    bool IsValidObject(MObject &obj);
    MObjectArray CollectNodes();
}


#endif // MEDIC_MAYA_MAYAAPI_INCLUDED