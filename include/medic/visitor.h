#ifndef MEDIC_VISITOR_INCLUDED
#define MEDIC_VISITOR_INCLUDED


#include <map>
#include "medic/karte.h"
#include "medic/report.h"
#include "medic/tester.h"
#include "medic/node.h"


namespace MEDIC
{
    class MdKarte;
    typedef std::map<MdTester *, MdReportContainer> TesterReportsMap;

    class MdVisitor
    {
        public:
            MdVisitor();
            virtual ~MdVisitor();
            virtual void visit(MdKarte *k);
            bool addReport(MdTester *tester, MdReport *report);
            void reset();
            std::vector<MdTester *> reportTesters();
            MdReportIterator report(MdTester *tester);
            MdNodeIterator nodes();
            TesterReportsMap &reportAll();

        protected:
            MdNodeContainer m_nodes;
            TesterReportsMap m_results;
    };
}


#endif