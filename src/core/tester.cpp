#include "medic/tester.h"


using namespace MEDIC;


MdTester::MdTester() {}

MdTester::~MdTester() {}

std::string MdTester::Name()
{
    return "Tester";
}

std::string MdTester::Description()
{
    return "";
};

std::vector<std::string> MdTester::Dependencies()
{
    return std::vector<std::string>();
}

void MdTester::initialize() {}

void MdTester::finalize() {}

bool MdTester::Match(MdNode *node)
{
    return false;
}

bool MdTester::IsFixable()
{
    return false;
};

MdParamContainer *MdTester::GetParameters()
{
    return new MdParamContainer();
};

MdReport *MdTester::test(MdNode *node)
{
    return 0;
};

bool MdTester::fix(MdReport *report, MdParamContainer *params)
{
    return true;
};


MdTesterContainer::MdTesterContainer() {}

MdTesterContainer::~MdTesterContainer() {}

std::vector<std::string> MdTesterContainer::names()
{
    std::vector<std::string> names;
    for (TesterPtrMap::iterator it = m_testers.begin(); it != m_testers.end(); ++it)
    {
        names.push_back(it->first);
    }

    return names;
}

MdTester *MdTesterContainer::get(std::string testerName)
{
    TesterPtrMap::iterator it = m_testers.find(testerName);
    if (it != m_testers.end())
    {
        return it->second;
    }

    return 0;
}

bool MdTesterContainer::hasTester(MdTester *t)
{
    TesterPtrMap::iterator it = m_testers.find(t->Name());
    if (it == m_testers.end())
    {
        return false;
    }

    return true;
}

bool MdTesterContainer::append(MdTester *t)
{
    TesterPtrMap::iterator it = m_testers.find(t->Name());
    if (it == m_testers.end())
    {
        m_testers[t->Name()] = t;
        return true;
    }

    return false;
}

bool MdTesterContainer::remove(MdTester *t, bool kill)
{
    TesterPtrMap::iterator it = m_testers.find(t->Name());
    if (it != m_testers.end())
    {
        if (kill)
        {
            delete it->second;
        }

        m_testers.erase(it);
        return true;
    }

    return false;
}

void MdTesterContainer::clear(bool kill)
{
    if (kill)
    {
        for (TesterPtrMap::iterator it = m_testers.begin(); it != m_testers.end(); ++it)
        {
            delete it->second;
        }
    }

    m_testers.clear();
}

MdTesterIterator MdTesterContainer::iterator()
{
    return MdTesterIterator(this);
}

size_t MdTesterContainer::size()
{
    return m_testers.size();
}


MdTesterIterator::MdTesterIterator() {}

MdTesterIterator::MdTesterIterator(MdTesterContainer *container)
{
    m_container = container;
    m_cur = m_container->m_testers.begin();
}

MdTesterIterator::~MdTesterIterator() {}

MdTester *MdTesterIterator::next()
{
    return (m_cur++)->second;
}

bool MdTesterIterator::isDone()
{
    return (m_cur == m_container->m_testers.end());
}