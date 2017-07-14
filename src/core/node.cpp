#include "medic/node.h"


using namespace MEDIC;


MdNode::MdNode() {}

MdNode::MdNode(MObject &obj)
{
    m_obj = obj;
    m_dg.setObject(obj);
    m_is_dag = false;

    if (obj.hasFn(MFn::kDagNode))
    {
        m_is_dag = true;
        m_dag.setObject(obj);
    }
}

MdNode::MdNode(std::string name)
{
    MSelectionList sel;
    sel.add(name.c_str());
    MObject obj;
    sel.getDependNode(0, obj);

    m_obj = obj;
    m_dg.setObject(obj);
    m_is_dag = false;

    if (obj.hasFn(MFn::kDagNode))
    {
        m_is_dag = true;
        m_dag.setObject(obj);
    }
}

MdNode::~MdNode() {}

bool MdNode::Match(MObject &obj)
{
    return (!obj.isNull() && obj.hasFn(MFn::kDependencyNode)) ? true : false;
}

MObject &MdNode::object()
{
    return m_obj;
}

std::string MdNode::name() const
{
    if (!isDag())
    {
        return (std::string)m_dg.name().asChar();
    }

    return (std::string)m_dag.partialPathName().asChar();
}


std::string MdNode::type() const
{
    return (std::string)m_dg.typeName().asChar();
}

bool MdNode::isDag() const
{
    return m_is_dag;
}

void MdNode::parents(MdNodeContainer *container) const
{
    if (isDag())
    {
        for (unsigned int i = 0; i < m_dag.parentCount(); ++i)
        {
            MObject po = m_dag.parent(i);
            container->append(new MdNode(po));
        }
    }
}

void MdNode::children(MdNodeContainer *container) const
{
    if (isDag())
    {
        for (unsigned int i = 0; i < m_dag.parentCount(); ++i)
        {
            MObject po = m_dag.child(i);
            container->append(new MdNode(po));
        }
    }
}

MFnDagNode &MdNode::dag()
{
    return m_dag;
}

MFnDependencyNode &MdNode::dg()
{
    return m_dg;
}

MDagPath MdNode::getPath() const
{
    if (!isDag())
    {
        return MDagPath();
    }

    MDagPath path;
    m_dag.getPath(path);

    return path;
}


MdNodeContainer::MdNodeContainer() {}

MdNodeContainer::~MdNodeContainer()
{
    clear();
}

bool MdNodeContainer::append(MdNode *n)
{
    if (std::find(m_nodes.begin(), m_nodes.end(), n) == m_nodes.end())
    {
        m_nodes.push_back(n);
        return true;
    }

    return false;
}

bool MdNodeContainer::remove(MdNode *n)
{
    NodePtrVec::iterator it = std::find(m_nodes.begin(), m_nodes.end(), n);
    if (it != m_nodes.end())
    {
        m_nodes.erase(it);
        delete (*it);
        return true;
    }

    return false;
}

void MdNodeContainer::clear()
{
    for (NodePtrVec::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
    {
        delete (*it);
    }

    m_nodes.clear();
}

size_t MdNodeContainer::size() const
{
    return m_nodes.size();
}

MdNodeIterator MdNodeContainer::iterator()
{
    return MdNodeIterator(this);
}

MdNodeIterator::MdNodeIterator() {}

MdNodeIterator::MdNodeIterator(MdNodeContainer *container)
{
    m_container = container;
    m_cur = m_container->m_nodes.begin();
}

MdNodeIterator::~MdNodeIterator() {}

MdNode *MdNodeIterator::next()
{
    return *(m_cur++);
}

bool MdNodeIterator::isDone()
{
    return (m_cur == m_container->m_nodes.end());
}


void MEDIC::GetNodes(MdNodeContainer *container)
{
    int i = 0;
    MItDependencyNodes it;
    while (!it.isDone())
    {
        std::cout << i << std::endl;

        MObject obj = it.thisNode();
        if (MdNode::Match(obj))
        {
            MFnDependencyNode dn(obj);
            if (!dn.isShared() && !dn.isDefaultNode() && dn.canBeWritten())
            {
                MdNode *n = new MdNode(obj);
                container->append(n);
            }
        }
        
        it.next();
    }
}
