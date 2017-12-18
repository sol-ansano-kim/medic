#include "medicMaya/mayaTester.h"


using namespace MEDIC;


MdMayaTester::MdMayaTester()
    : MdTester() {}

MdMayaTester::~MdMayaTester() {}

bool MdMayaTester::Match(const MdNode *node) const
{
    const MdMayaNode *maya_node = dynamic_cast<const MdMayaNode *>(node);

    if (maya_node == NULL)
    {
        return false;    
    }

    return Match(maya_node);    
}

bool MdMayaTester::Match(const MdMayaNode *node) const
{
    return false;
}

MdReport *MdMayaTester::test(const MdNode *node) const
{
    const MdMayaNode *maya_node = dynamic_cast<const MdMayaNode *>(node);
    MdMayaReport *report = test(maya_node);

    return (report != NULL) ? (MdReport *)report : NULL;
}

MdMayaReport *MdMayaTester::test(const MdMayaNode *node) const
{
    return NULL;
}

bool MdMayaTester::fix(const MdReport *report, MdParamContainer *params) const
{
    const MdMayaReport *maya_report = dynamic_cast<const MdMayaReport *>(report);
    if (maya_report == NULL)
    {
        return false;
    }

    return fix(maya_report, params);
}

bool MdMayaTester::fix(const MdMayaReport *report, MdParamContainer *params) const
{
    return false;
}