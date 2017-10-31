#include "medic/node.h"


using namespace MEDIC;


MdNode::MdNode() {}

MdNode::MdNode(std::string name, std::string type) : m_name(name), m_type(type) {}

MdNode::~MdNode() {}

std::string MdNode::name() const
{
    return m_name;
}

std::string MdNode::type() const
{
    return m_type;
}
