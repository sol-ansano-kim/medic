#ifndef MEDIC_MAYA_MAYATESTER_INCLUDED
#define MEDIC_MAYA_MAYATESTER_INCLUDED


#include "medic/maya/mayaNode.h"
#include "medic/maya/mayaReport.h"
#include "medic/tester.h"


namespace MEDIC
{
    class MdMayaTester : public MdTester
    {
        public:
            MdMayaTester();
            virtual ~MdMayaTester();
            bool Match(const MdNode *node) const;
            MdReport *test(const MdNode *node) const;
            bool fix(const MdReport *report, MdParamContainer *params) const;

            virtual bool Match(const MdMayaNode *node) const;
            virtual MdMayaReport *test(const MdMayaNode *node) const;
            virtual bool fix(const MdMayaReport *report, MdParamContainer *params) const;
    };
}


#endif // MEDIC_MAYA_MAYATESTER_INCLUDED