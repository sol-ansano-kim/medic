#include "medic/karte.h"


using namespace MEDIC;


MdKarte::MdKarte(std::string name, std::string desc)
: m_name(name), m_description(desc) {}

MdKarte::~MdKarte()
{
    m_testers.clear();
}

MdTester *MdKarte::tester(std::string testerName)
{
    return m_testers.get(testerName);
}

bool MdKarte::hasTester(MdTester *t)
{
    return m_testers.hasTester(t);
}

std::string MdKarte::Name() const
{
    return m_name;
}

std::string MdKarte::Description() const
{
    return m_description;
}

bool MdKarte::addTester(MdTester *tester)
{
    return m_testers.append(tester);
}

void MdKarte::accept(MdVisitor *v)
{
    v->visit(this);
}

MdTesterIterator MdKarte::testers()
{
    return m_testers.iterator();
}


MdKarteContainer::MdKarteContainer() {}

MdKarteContainer::~MdKarteContainer() {}

std::vector<std::string> MdKarteContainer::names()
{
    std::vector<std::string> names;
    for (KartePtrMap::iterator it = m_kartes.begin(); it != m_kartes.end(); ++it)
    {
        names.push_back(it->first);
    }

    return names;
}

MdKarte *MdKarteContainer::get(std::string karteName)
{
    KartePtrMap::iterator it = m_kartes.find(karteName);
    if (it != m_kartes.end())
    {
        return it->second;
    }

    return 0;
}

bool MdKarteContainer::append(MdKarte *k)
{
    KartePtrMap::iterator it = m_kartes.find(k->Name());
    if (it == m_kartes.end())
    {
        m_kartes[k->Name()] = k;
        return true;
    }

    return false;
}

bool MdKarteContainer::remove(MdKarte *k, bool kill)
{
    KartePtrMap::iterator it = m_kartes.find(k->Name());
    if (it != m_kartes.end())
    {
        if (kill)
        {
            delete it->second;
        }

        m_kartes.erase(it);
        return true;
    }

    return false;
}

void MdKarteContainer::clear(bool kill)
{
    if (kill)
    {
        for (KartePtrMap::iterator it = m_kartes.begin(); it != m_kartes.end(); ++it)
        {
            delete it->second;
        }
    }

    m_kartes.clear();
}

MdKarteIterator MdKarteContainer::iterator()
{
    return MdKarteIterator(this);
}

size_t MdKarteContainer::size()
{
    return m_kartes.size();
}


MdKarteIterator::MdKarteIterator(MdKarteContainer *container)
{
    m_container = container;
    m_cur = m_container->m_kartes.begin();
}

MdKarteIterator::~MdKarteIterator() {}

MdKarte *MdKarteIterator::next()
{
    return (m_cur++)->second;
}

bool MdKarteIterator::isDone()
{
    return (m_cur == m_container->m_kartes.end());
}