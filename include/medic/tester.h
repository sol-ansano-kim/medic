#ifndef MEDIC_TESTER_INCLUDED
#define MEDIC_TESTER_INCLUDED


#include <string>
#include "medic/parameter.h"
#include "medic/node.h"


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
            virtual bool Match(const MdNode *node) const;
            virtual bool IsFixable() const;
            virtual MdParamContainer *GetParameters() const;
            virtual MdReport *test(const MdNode *node) const;
            virtual bool fix(const MdReport *report, MdParamContainer *params) const;

        protected:
            bool m_isfixable;
            std::string m_name;
            std::string m_description;
    };
}


#endif // MEDIC_TESTER_INCLUDED