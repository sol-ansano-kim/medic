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

        private:
            MdPluginManager();
            ~MdPluginManager();
            void searchKartes();
            void searchTesters();
            void loadPlugins();
            void unloadPlugins();

        private:
            static MdPluginManager *m_instance;
            std::map<std::string, MdKarte *> m_kartes;
            std::map<std::string, MdTester *> m_testers;
            std::vector<void *> m_dls;
    };


}

#endif // MEDIC_PLUGINMANAGER_INCLUDED