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
            virtual void visit(MdKarte *karte);
            virtual bool visit(MdKarte *karte, MdTester *tester);
            bool addReport(MdTester *tester, MdReport *report);
            void reset();
            std::vector<MdTester *> reportTesters();
            MdReportIterator report(MdTester *tester);
            MdNodeIterator nodes();
            TesterReportsMap &reportAll();

        protected:
            void cleanReport(MdTester *tester);
            MdNodeContainer m_nodes;
            TesterReportsMap m_results;
            bool m_node_collected;
    };
}


#endif