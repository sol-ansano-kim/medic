#ifndef MEDIC_NODE_INCLUDED
#define MEDIC_NODE_INCLUDED


#include <Python.h>
#include <string>


namespace MEDIC
{
	class MdNode
	{
        public:
            MdNode();
            virtual ~MdNode();
            virtual std::string name() const;
            virtual std::string type() const;
            virtual PyObject *getPythonObject() const;

        protected:
            std::string m_name;
            std::string m_type;
            PyObject *m_pyobj;
	};
}


#endif // MEDIC_NODE_INCLUDED