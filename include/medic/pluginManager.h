#ifndef MEDIC_PLUGINMANAGER_INCLUDED
#define MEDIC_PLUGINMANAGER_INCLUDED


#include "medic/tester.h"
#include "medic/karte.h"
#include <string>
#include <vector>


namespace MEDIC
{
    enum MdPluginLoadingStatus
    {
        MdLoadingFailure = 0,
        MdLoadingSuccess,
        MdExistsPlugin,
    };

    class MdPlugInManager
    {
        public:
            static MdPlugInManager *Instance();
            void testers(MdTesterContainer *container);
            std::vector<std::string> testerNames();
            std::vector<std::string> karteNames();
            MdTester *tester(std::string name);
            MdKarte *karte(std::string name);
            MdPluginLoadingStatus addTester(std::string pluginPath);
            MdPluginLoadingStatus addKarte(std::string name, std::string description, std::vector<std::string> testerNames);
            void unload();

        private:
            MdPlugInManager();
            ~MdPlugInManager();
            
            static bool m_isntanced;
            static MdPlugInManager *m_manager;
            MdTesterContainer m_testers;
            MdKarteContainer m_kartes;
            std::vector<void *> m_dls;
            std::vector<std::string> m_plugin_path_cache;
    };
}


#endif 