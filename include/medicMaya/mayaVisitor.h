#ifndef MEDIC_MAYA_MAYAVISITOR_INCLUDED
#define MEDIC_MAYA_MAYAVISITOR_INCLUDED


#include "medicMaya/mayaAPI.h"
#include "medicMaya/mayaNode.h"
#include "medic/pluginManager.h"
#include "medic/visitor.h"


namespace MEDIC
{
    class MdMayaVisitor : public MdVisitor
    {
        public:
            MdMayaVisitor(const MdPluginManager *manager);
            virtual ~MdMayaVisitor();
            virtual bool setKarte(const MdKarte *karte);
            virtual void collectNodes();
    };
}


#endif // MEDIC_MAYA_MAYAVISITOR_INCLUDED