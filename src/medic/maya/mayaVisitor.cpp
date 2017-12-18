#include <maya/MObjectArray.h>
#include "medic/maya/mayaVisitor.h"


using namespace MEDIC;


MdMayaVisitor::MdMayaVisitor(const MdPluginManager *manager)
    : MdVisitor(manager) {}

MdMayaVisitor::~MdMayaVisitor() {}

void MdMayaVisitor::collectNodes()
{
    MObjectArray obj_array = MEDIC::CollectNodes();

    for (unsigned int i = 0; i < obj_array.length(); ++i)
    {
        MdNode *node = new MdMayaNode(obj_array[i]);
        m_nodes.push_back(node);
    }
}