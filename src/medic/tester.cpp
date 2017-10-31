#include "medic/tester.h"


using namespace MEDIC;


MdTester::MdTester() {}

MdTester::~MdTester() {}

std::string MdTester::Name() const
{
    return "Tester";
}

std::string MdTester::Description() const
{
    return "";
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

MdReport *MdTester::test(MdNode *node) const
{
    return 0;
};

bool MdTester::fix(MdReport *report, MdParamContainer *params) const
{
    return true;
};
