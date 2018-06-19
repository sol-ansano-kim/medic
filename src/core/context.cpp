#include "medic/context.h"

using namespace MEDIC;


MdContext::MdContext(const std::string& name)
    : m_name(name)
{
    m_params = new MdParamContainer();
}

MdContext::MdContext(const std::string& name, MdParamContainer* params)
    : m_name(name), m_params(params) {}

MdContext::~MdContext()
{
    if (m_params)
    {
        delete m_params;
    }
} 

std::string MdContext::name() const
{
    return m_name;
}

MdParamContainer* MdContext::params()
{
    return m_params;
}

MdParameter* MdContext::param(const std::string& name)
{
    return m_params->getParam(name);
}

bool MdContext::addParam(MdParameter* param)
{
    return m_params->append(param);
}


MdContextContainer::MdContextContainer() {}

MdContextContainer::~MdContextContainer()
{
    clear();
}

bool MdContextContainer::append(MdContext* c)
{
    if (std::find(m_contexts.begin(), m_contexts.end(), c) == m_contexts.end())
    {
        m_contexts.push_back(c);
        return true;
    }

    return false;
}

bool MdContextContainer::remove(MdContext* c)
{
    ContextPtrVec::iterator it = std::find(m_contexts.begin(), m_contexts.end(), c);
    if (it != m_contexts.end())
    {
        m_contexts.erase(it);
        delete (*it);
        return true;
    }

    return false;
}

void MdContextContainer::clear()
{
    for (ContextPtrVec::iterator it = m_contexts.begin(); it != m_contexts.end(); ++it)
    {
        delete (*it);
    }

    m_contexts.clear();
}

size_t MdContextContainer::size() const
{
    return m_contexts.size();
}

MdContextIterator MdContextContainer::iterator()
{
    return MdContextIterator(this);
}


MdContextIterator::MdContextIterator() {}

MdContextIterator::MdContextIterator(MdContextContainer* container)
{
    m_container = container;
    m_cur = m_container->m_contexts.begin();
}

MdContextIterator::~MdContextIterator() {}

MdContext *MdContextIterator::next()
{
    return *(m_cur++);
}

bool MdContextIterator::isDone()
{
    return (m_cur == m_container->m_contexts.end());
}
