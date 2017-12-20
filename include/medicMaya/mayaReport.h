#ifndef MEDICMAYA_MAYAREPORT_INCLUDED
#define MEDICMAYA_MAYAREPORT_INCLUDED


#include <maya/MObject.h>
#include "medic/report.h"


namespace MEDIC
{
    class MdMayaNode;

    class MdMayaReport : public MdReport
    {
        public:
            MdMayaReport(const MdMayaNode *node);
            MdMayaReport(const MdMayaNode *node, MObject &components);
            virtual ~MdMayaReport();
            const MObject &components() const;
            bool hasComponents() const;

        private:
            bool m_has_components;
            const MdNode *m_node;
            const MObject m_components;
    };
}


#endif // MEDICMAYA_MAYAREPORT_INCLUDED