#ifndef MEDIC_PLUGINMANAGER_INCLUDED
#define MEDIC_PLUGINMANAGER_INCLUDED


#include "medic/tester.h"
#include "medic/karte.h"
#include "medic/platform.h"
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
            MEDIC_EXPORT static MdPlugInManager *Instance();
            void testers(MdTesterContainer *container);
            MEDIC_EXPORT std::vector<std::string> testerNames();
            std::vector<std::string> karteNames();
            MEDIC_EXPORT MdTester *tester(std::string name);
            MEDIC_EXPORT MdKarte *karte(std::string name);
            MEDIC_EXPORT MdPluginLoadingStatus addTester(std::string pluginPath);
            MEDIC_EXPORT MdPluginLoadingStatus addKarte(std::string name, std::string description, std::vector<std::string> testerNames);
            MEDIC_EXPORT void unload();

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