#ifndef MEDIC_NODE_INCLUDED
#define MEDIC_NODE_INCLUDED


#include <string>


namespace MEDIC
{
	class MdNode
	{
        public:
            MdNode(const std::string &name, const std::string &type);
            virtual ~MdNode();
            virtual std::string name() const;
            virtual std::string type() const;

        protected:
            std::string m_name;
            std::string m_type;
	};
}


#endif // MEDIC_NODE_INCLUDED