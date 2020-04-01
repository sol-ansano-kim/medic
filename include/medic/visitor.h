#ifndef MEDIC_VISITOR_INCLUDED
#define MEDIC_VISITOR_INCLUDED


#include <string>
#include <map>
#include "medic/karte.h"
#include "medic/report.h"
#include "medic/tester.h"
#include "medic/node.h"
#include "medic/context.h"
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
            virtual void visitKarte(MdKarte *karte);
            virtual void visitTester(MdTester *tester);
            virtual void visitNode(MdTester *tester, MdNode *node);
            virtual void visitContext(MdTester *tester, MdContext *context);
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
            MEDIC_EXPORT bool setScene(MdContext *scene);
            MEDIC_EXPORT MdContext *scene();
            MEDIC_EXPORT bool addAsset(MdContext *asset);
            MEDIC_EXPORT MdContextIterator assets();
            MEDIC_EXPORT void clearScene();
            MEDIC_EXPORT void clearAssets();
            MEDIC_EXPORT bool selectionOnly() const;
            MEDIC_EXPORT void setSelectionOnly(bool v);
            MEDIC_EXPORT void initializeTester(MdTester *tester);
            MEDIC_EXPORT void finalizeTester(MdTester *tester);

        private:
            static bool warned_visit_karter;
            static bool warned_visit_tester;

            bool m_node_collected;
            bool m_collect_in_selection;
            MdContext *m_scene;
            MdContextContainer m_assets;
            MdNodeContainer m_nodes;
            TesterReportsMap m_results;
            OptionMaps m_options;

            void cleanReport(MdTester *tester);
            void collectNodes();
    };
}


#endif