#ifndef MEDIC_VISITOR_INCLUDED
#define MEDIC_VISITOR_INCLUDED


#include <string>
#include <map>
#include "medic/karte.h"
#include "medic/report.h"
#include "medic/tester.h"
#include "medic/node.h"
#include "medic/platform.h"


namespace MEDIC
{
    class MdKarte;
    typedef std::map<MdTester *, MdReportContainer> TesterReportsMap;
    typedef std::map<std::string, MdParamContainer*> OptionMaps;

    class MdVisitor
    {
        public:
            MEDIC_EXPORT MdVisitor();
            MEDIC_EXPORT virtual ~MdVisitor();
            virtual void visit(MdKarte *karte);
            virtual bool visit(MdKarte *karte, MdTester *tester);
            bool addReport(MdTester *tester, MdReport *report);
            MEDIC_EXPORT void reset();
            MEDIC_EXPORT std::vector<MdTester *> reportTesters();
            MEDIC_EXPORT MdReportIterator report(MdTester *tester);
            MEDIC_EXPORT bool hasError(MdTester *tester);
            MEDIC_EXPORT MdNodeIterator nodes();
            TesterReportsMap &reportAll();
            MEDIC_EXPORT void clearOptions();
            MEDIC_EXPORT MdParamContainer* getOptions(const std::string& name);
            MEDIC_EXPORT std::vector<std::string> getOptionKeys();

        protected:
            bool m_node_collected;
            MdNodeContainer m_nodes;
            TesterReportsMap m_results;
            OptionMaps m_options;

            void cleanReport(MdTester *tester);
    };
}


#endif