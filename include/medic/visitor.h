#ifndef MEDIC_VISITOR_INCLUDED
#define MEDIC_VISITOR_INCLUDED


#include <map>
#include <vector>
#include <string>
#include "medic/node.h"
#include "medic/report.h"
#include "medic/tester.h"
#include "medic/karte.h"


namespace MEDIC
{
    typedef std::map<const std::string &, const MdTester *> NameTesterMap;
    typedef std::vector<MdReport *> ReportPtrVec;
    typedef std::map<const MdTester *, ReportPtrVec> TesterReportsMap;
    typedef std::vector<MdNode *> NodePtrVec;

    class MdVisitor
    {
        public:
            MdVisitor(const MdKarte *karte);
            ~MdVisitor();
            const MdTester *getTester(const std::string &name);
            std::vector<const MdTester *> getTesters();
            bool hasTester(const MdTester *tester);
            bool hasTester(const std::string &name);
            virtual void test(const std::string &name);
            virtual void test(const MdTester *tester);
            virtual void testAll();
            virtual void reset();
            virtual void collectNodes();
            virtual void clearNodes();

        protected:
            virtual void addReport(const MdTester *tester, MdReport *report);
            virtual void clearReports(const MdTester *tester);
            virtual void clearAllReports();

            const MdKarte *m_karte;
            NodePtrVec m_nodes;
            NameTesterMap m_testers;
            TesterReportsMap m_reports;
    };
}


#endif // MEDIC_VISITOR_INCLUDED