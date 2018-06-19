#ifndef MEDIC_CONTEXT_INCLUDED
#define MEDIC_CONTEXT_INCLUDED


#include <string>
#include "medic/parameter.h"


namespace MEDIC
{
    class MdContext;
    class MdContextContainer;
    class MdContextIterator;
    typedef std::vector<MdContext*> ContextPtrVec;

    class MdContext
    {
        public:
            MEDIC_EXPORT MdContext(const std::string& name);
            MEDIC_EXPORT MdContext(const std::string& name, MdParamContainer* params);
            MEDIC_EXPORT ~MdContext();
            MEDIC_EXPORT std::string name() const;
            MEDIC_EXPORT MdParamContainer* params();
            MdParameter* param(const std::string& name);
            bool addParam(MdParameter* param);

        private:
            std::string m_name;
            MdParamContainer* m_params;
    };

    class MdContextContainer
    {
        friend class MdContextIterator;
        public:
            MEDIC_EXPORT MdContextContainer();
            MEDIC_EXPORT ~MdContextContainer();
            bool append(MdContext* c);
            bool remove(MdContext* c);
            void clear();
            size_t size() const;
            MEDIC_EXPORT MdContextIterator iterator();

        private:
            ContextPtrVec m_contexts;
    };

    class MdContextIterator
    {
        public:
            MEDIC_EXPORT MdContextIterator();
            MEDIC_EXPORT MdContextIterator(MdContextContainer *container);
            MEDIC_EXPORT ~MdContextIterator();
            MEDIC_EXPORT MdContext* next();
            MEDIC_EXPORT bool isDone();

        private:
            MdContextContainer* m_container;
            ContextPtrVec::iterator m_cur;
    };
}


#endif