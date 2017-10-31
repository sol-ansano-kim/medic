#include "medic/report.h"


using namespace MEDIC;


MdReport::MdReport(MdNode *node)
{
    m_node = node;
}

MdReport::~MdReport() {};

MdNode *MdReport::node() const
{
    return m_node;
}
