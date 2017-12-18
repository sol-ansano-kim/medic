#ifndef MEDIC_MAYA_MAYANODE_INCLUDED
#define MEDIC_MAYA_MAYANODE_INCLUDED


#include <Python.h>
#include <string>
#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include "medic/node.h"


namespace MEDIC
{
    class MdMayaNode : public MdNode
    {
        public:
            MdMayaNode();
            MdMayaNode(MObject &obj);
            MdMayaNode(std::string name);
            ~MdMayaNode();
            std::string name() const;
            std::string type() const;
            bool isDag() const;
            // void parents(MdNodeContainer *container) const;
            // void children(MdNodeContainer *container) const;
            MDagPath getPath();
            MObject &object();
            MFnDependencyNode &dg();
            MFnDagNode &dag();
            PyObject *pyGetPath();
            PyObject *pyObject();
            PyObject *pyDg();
            PyObject *pyDag();

        private:
            bool m_is_dag;
            MObject m_obj;
            MFnDependencyNode m_dg;
            MFnDagNode m_dag;
            PyObject *m_py_obj;
            PyObject *m_py_dg;
            PyObject *m_py_dag;
            PyObject *m_py_path;
    };    
}


#endif // MEDIC_MAYA_MAYANODE_INCLUDED
