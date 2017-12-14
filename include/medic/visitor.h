#ifndef MEDIC_VISITOR_INCLUDED
#define MEDIC_VISITOR_INCLUDED


#include "medic/node.h"
#include "medic/report.h"
#include "medic/tester.h"
#include "medic/karte.h"
#include "medic/pluginManager.h"
#include <map>
#include <vector>
#include <string>


namespace MEDIC
{
    typedef std::map<std::string, const MdTester *> NameTesterMap;
    typedef std::vector<const MdReport *> ReportPtrVec;
    typedef std::map<const MdTester *, ReportPtrVec> TesterReportsMap;
    typedef std::vector<const MdNode *> NodePtrVec;

    class MdVisitor
    {
        public:
            MdVisitor(const MdPluginManager *manager);
            ~MdVisitor();
            bool setKarte(const std::string &name);
            bool setKarte(const MdKarte *karte);
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

            const MdPluginManager *m_manager;
            const MdKarte *m_karte;
            NodePtrVec m_nodes;
            NameTesterMap m_testers;
            TesterReportsMap m_reports;
    };
}


#endif // MEDIC_VISITOR_INCLUDED