#include "medic/maya/mayaReport.h"


using namespace MEDIC;


MdMayaReport::MdMayaReport(const MdMayaNode *node)
    : MdReport((const MEDIC::MdNode *)node), m_has_components(false) {}

MdMayaReport::MdMayaReport(const MdMayaNode *node, MObject &component)
    : MdReport((const MEDIC::MdNode *)node), m_has_components(true), m_components(component) {}

MdMayaReport::~MdMayaReport() {}

const MObject &MdMayaReport::component() const
{
    return m_components;
}

bool MdMayaReport::hasComponents() const
{
    return m_has_components;
}