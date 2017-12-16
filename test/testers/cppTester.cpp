#include "medic/plugin.h"


using namespace MEDIC;


class TestDL : public MdTester
{
    public:
        TestDL();
        virtual ~TestDL();
};

TestDL::TestDL()
{
    m_isfixable = false;
    m_name = "TesterDL";
    m_description = "cpp dl tester";
}


TestDL::~TestDL() {};


MEDIC_TESTER
{
    return new TestDL();
}