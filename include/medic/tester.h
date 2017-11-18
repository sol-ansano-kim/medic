#ifndef MEDIC_TESTER_INCLUDED
#define MEDIC_TESTER_INCLUDED


#include <string>
#include "medic/parameter.h"


namespace MEDIC
{
    class MdNode;
    class MdReport;

    class MdTester
    {
        public:
            MdTester();
            virtual ~MdTester();
            virtual const std::string &Name() const;
            virtual const std::string &Description() const;
            virtual bool Match(MdNode *node) const;
            virtual bool IsFixable() const;
            virtual MdParamContainer *GetParameters() const;
            virtual MdReport *test(const MdNode *node) const;
            virtual bool fix(MdReport *report, MdParamContainer *params) const;

        private:
            static const std::string m_unknown;
    };
}


#endif