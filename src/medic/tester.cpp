#include "medic/tester.h"


using namespace MEDIC;


MdTester::MdTester()
    : m_isfixable(false), m_name("UNKNOWN"), m_description("UNKNOWN") {}

MdTester::~MdTester() {}

const std::string &MdTester::Name() const
{
    return m_name;
}

const std::string &MdTester::Description() const
{
    return m_description;
};

bool MdTester::Match(const MdNode *node) const
{
    return false;
}

bool MdTester::IsFixable() const
{
    return m_isfixable;
};

MdParamContainer *MdTester::GetParameters() const
{
    return new MdParamContainer();
};

MdReport *MdTester::test(const MdNode *node) const
{
    return NULL;
};

bool MdTester::fix(const MdReport *report, MdParamContainer *params) const
{
    return false;
};
