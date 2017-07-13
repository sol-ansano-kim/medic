#ifndef MEDIC_TESTER_INCLUDED
#define MEDIC_TESTER_INCLUDED


#include <string>
#include <map>
#include "medic/node.h"
#include "medic/parameter.h"
#include "medic/report.h"


namespace MEDIC
{
    class MdTester;
    class MdTesterContainer;
    class MdTesterIterator;

    class MdTester
    {
        public:
            MdTester();
            virtual ~MdTester();
            virtual std::string Name();
            virtual std::string Description();
            virtual bool Match(MdNode *node);
            virtual bool IsFixable();
            virtual MdParamContainer *GetParameters();
            virtual MdReport *test(MdNode *node);
            virtual bool fix(MdReport *report, MdParamContainer *params);
    };

    typedef std::map<std::string, MdTester *> TesterPtrMap;

    class MdTesterContainer
    {
        friend class MdTesterIterator;

        public:
            MdTesterContainer();
            ~MdTesterContainer();
            std::vector<std::string> names();
            MdTester *get(std::string testerName);
            bool append(MdTester *t);
            bool remove(MdTester *t, bool kill=false);
            void clear(bool kill=false);
            size_t size();
            MdTesterIterator iterator();

        private:
            TesterPtrMap m_testers;
    };

    class MdTesterIterator
    {
        public:
            MdTesterIterator(MdTesterContainer *container);
            ~MdTesterIterator();
            MdTester *next();
            bool isDone();

        private:
            MdTesterContainer *m_container;
            TesterPtrMap::iterator m_cur;
    };
}


#endif