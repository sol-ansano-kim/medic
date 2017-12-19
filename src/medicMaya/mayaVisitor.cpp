#include <maya/MObjectArray.h>
#include "medicMaya/mayaVisitor.h"


using namespace MEDIC;


MdMayaVisitor::MdMayaVisitor(const MdPluginManager *manager)
    : MdVisitor(manager) {}

MdMayaVisitor::~MdMayaVisitor() {}

bool MdMayaVisitor::setKarte(const MdKarte *karte)
{
    clearAllReports();
    m_testers.clear();

    m_karte = karte;
    debug("Set karte : " + karte->Name());

    std::vector<const MdTester *> testers = m_manager->getTesters(karte);
    for (std::vector<const MdTester *>::iterator it = testers.begin(); it != testers.end(); ++it)
    {
        const MdMayaTester *maya_tester = dynamic_cast<const MdMayaTester *>(*it);
        if (maya_tester == NULL)
        {
            debug("It is not a tester for maya : " + (*it)->Name());
            continue;
        }

        debug("Add tester : " + (*it)->Name());
        m_testers[(*it)->Name()] = *it;
    }

    return true;
}

void MdMayaVisitor::collectNodes()
{
    MObjectArray obj_array = MEDIC::CollectNodes();

    for (unsigned int i = 0; i < obj_array.length(); ++i)
    {
        MdNode *node = new MdMayaNode(obj_array[i]);
        m_nodes.push_back(node);
    }
}