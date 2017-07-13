#ifndef MEDIC_KARTE_INCLUDED
#define MEDIC_KARTE_INCLUDED


#include <string>
#include <map>
#include "medic/tester.h"
#include "medic/visitor.h"


namespace MEDIC
{
    class MdVisitor;
    class MdKarte;
    class MdKarteContainer;
    class MdKarteIterator;
    typedef std::map<std::string, MdKarte *> KartePtrMap;

    class MdKarte
    {
        public:
            MdKarte(std::string name, std::string desc);
            ~MdKarte();
            MdTester *tester(std::string testerName);
            bool hasTester(MdTester *t);
            std::string Name() const;
            std::string Description() const;
            bool addTester(MdTester *tester);
            void accept(MdVisitor *v);
            MdTesterIterator testers();

        private:
            MdTesterContainer m_testers;
            std::string m_name;
            std::string m_description;
    };

    class MdKarteContainer
    {
        friend class MdKarteIterator;

        public:
            MdKarteContainer();
            ~MdKarteContainer();
            std::vector<std::string> names();
            MdKarte *get(std::string karteName);
            bool append(MdKarte *t);
            bool remove(MdKarte *t, bool kill=false);
            void clear(bool kill=false);
            size_t size();
            MdKarteIterator iterator();

        private:
            KartePtrMap m_kartes;
    };

    class MdKarteIterator
    {
        public:
            MdKarteIterator(MdKarteContainer *container);
            ~MdKarteIterator();
            MdKarte *next();
            bool isDone();

        private:
            MdKarteContainer *m_container;
            KartePtrMap::iterator m_cur;
    };
}


#endif