#include "medic/node.h"


using namespace MEDIC;


MdNode::MdNode() {}

MdNode::MdNode(const std::string &name, const std::string &type) : m_name(name), m_type(type) {}

MdNode::~MdNode() {}

const std::string &MdNode::name() const
{
    return m_name;
}

const std::string &MdNode::type() const
{
    return m_type;
}
