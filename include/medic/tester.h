#ifndef MEDIC_TESTER_INCLUDED
#define MEDIC_TESTER_INCLUDED


#include <string>
#include <vector>
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
            MEDIC_EXPORT MdTester();
            MEDIC_EXPORT virtual ~MdTester();
            MEDIC_EXPORT virtual std::string Name();
            MEDIC_EXPORT virtual std::string Description();
            MEDIC_EXPORT virtual std::vector<std::string> Dependencies();
            MEDIC_EXPORT virtual bool Match(MdNode *node);
            MEDIC_EXPORT virtual void initialize();
            MEDIC_EXPORT virtual void finalize();
            MEDIC_EXPORT virtual bool IsFixable();
            MEDIC_EXPORT virtual MdParamContainer *GetParameters();
            MEDIC_EXPORT virtual MdReport *test(MdNode *node);
            MEDIC_EXPORT virtual bool fix(MdReport *report, MdParamContainer *params);
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
            bool hasTester(MdTester *t);
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
            MEDIC_EXPORT MdTesterIterator();
            MdTesterIterator(MdTesterContainer *container);
            ~MdTesterIterator();
            MEDIC_EXPORT MdTester *next();
            MEDIC_EXPORT bool isDone();

        private:
            MdTesterContainer *m_container;
            TesterPtrMap::iterator m_cur;
    };
}


#endif