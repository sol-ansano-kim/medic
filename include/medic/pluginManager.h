#ifndef MEDIC_PLUGINMANAGER_INCLUDED
#define MEDIC_PLUGINMANAGER_INCLUDED


#include <string>
#include <map>
#include <vector>
#include "medic/platform.h"
#include "medic/karte.h"
#include "medic/tester.h"
#include "medic/pytester.h"


namespace MEDIC
{
    class MdPluginManager
    {
        public:
            static MdPluginManager *Instance();
            void reloadPlugins();
            const MdKarte *getKarte(const std::string &name) const;
            const MdTester *getTester(const std::string &name) const;
            std::vector<std::string> getKarteNames() const;
            std::vector<std::string> getTesterNames() const;
            std::vector<const MdTester *> getTesters(const MdKarte *karte) const;

        private:
            MdPluginManager();
            ~MdPluginManager();
            void searchKartes();
            void searchTesters();
            void loadPlugins();
            void unloadPlugins();

        private:
            static MdPluginManager *m_instance;
            std::map<const std::string, const MdKarte *> m_kartes;
            std::map<const std::string, const MdTester *> m_testers;
            std::vector<void *> m_dls;
    };


}

#endif // MEDIC_PLUGINMANAGER_INCLUDED