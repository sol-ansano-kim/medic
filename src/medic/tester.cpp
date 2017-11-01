#include "medic/tester.h"


using namespace MEDIC;


const std::string MdTester::m_unknown("UNKNOWN");


MdTester::MdTester() {}

MdTester::~MdTester() {}

const std::string &MdTester::Name() const
{
    return m_unknown;
}

const std::string &MdTester::Description() const
{
    return m_unknown;
};

bool MdTester::Match(MdNode *node) const
{
    return false;
}

bool MdTester::IsFixable() const
{
    return false;
};

MdParamContainer *MdTester::GetParameters() const
{
    return new MdParamContainer();
};

MdReport *MdTester::test(const MdNode *node) const
{
    return NULL;
};

bool MdTester::fix(MdReport *report, MdParamContainer *params) const
{
    return false;
};
