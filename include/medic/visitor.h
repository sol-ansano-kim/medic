#ifndef MEDIC_VISITOR_INCLUDED
#define MEDIC_VISITOR_INCLUDED


#include <map>
#include <vector>
#include <string>


namespace MEDIC
{
    class MdKarte;
    class MdTester;
    class MdReport;
    class MdNode;

    class MdVisitor
    {
        public:
            MdVisitor(MdKarte *karte);
            virtual ~MdVisitor();
            virtual void visit();
            virtual void visit(MdTester *tester);
            void reset();

        protected:
            MdKarte *m_karte;
            std::vector<MdNode *> m_nodes;
            std::map<MdTester *, std::vector<MdReport *> > m_results;
            std::map<std::string, MdTester *> m_testers;
    };
}


#endif