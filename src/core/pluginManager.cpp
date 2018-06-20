#include "medic/pluginManager.h"


using namespace MEDIC;


typedef MdTester* (*CreateTesterFuncType)();
bool MdPlugInManager::m_isntanced = false;
MdPlugInManager *MdPlugInManager::m_manager = 0;


MdPlugInManager *MdPlugInManager::Instance()
{
    if (!m_isntanced)
    {
        m_manager = new MdPlugInManager();
        m_isntanced = true;   
    }

    return m_manager;
}

void MdPlugInManager::testers(MdTesterContainer *container)
{
    MdTesterIterator it = m_testers.iterator();

    while (!it.isDone())
    {
        container->append(it.next());
    }
}

std::vector<std::string> MdPlugInManager::testerNames()
{
    return m_testers.names();
}

std::vector<std::string> MdPlugInManager::karteNames()
{
    return m_kartes.names();
}

MdTester *MdPlugInManager::tester(std::string name)
{
    return m_testers.get(name);
}

MdKarte *MdPlugInManager::karte(std::string name)
{
    return m_kartes.get(name);
}

MdPluginLoadingStatus MdPlugInManager::addTester(std::string pluginPath)
{
    std::vector<std::string>::iterator it = std::find(m_plugin_path_cache.begin(), m_plugin_path_cache.end(), pluginPath);
    if (it != m_plugin_path_cache.end())
    {
        return MdExistsPlugin;
    }

    m_plugin_path_cache.push_back(pluginPath);

    void *dl = dlopen(pluginPath.c_str(), RTLD_LAZY|RTLD_LOCAL);
    if (dl)
    {
        CreateTesterFuncType regFunc = (CreateTesterFuncType) dlsym(dl, "Create");
        if (regFunc)
        {
            MdTester *t = regFunc();
            m_dls.push_back(dl);
            if (m_testers.append(t))
            {
                return MdLoadingSuccess;
            }

            return MdExistsPlugin;
        }
        else
        {
            dlclose(dl);
        }
    }

    return MdLoadingFailure;
}

MdPluginLoadingStatus MdPlugInManager::addKarte(std::string name, std::string description, std::vector<std::string> testerNames)
{
    MdKarte *exist = m_kartes.get(name);
    if (exist)
    {
        return MdExistsPlugin;
    }

    MdKarte *karte = new MdKarte(name, description);

    for (std::vector<std::string>::iterator it = testerNames.begin(); it != testerNames.end(); ++it)
    {
        MdTester *t = m_testers.get(*it);
        if (t)
        {
            karte->addTester(t);
        }
    }

    m_kartes.append(karte);

    return MdLoadingSuccess;
}

MdPlugInManager::MdPlugInManager() {}

MdPlugInManager::~MdPlugInManager()
{
    m_isntanced = false;
    unload();
}

void MdPlugInManager::unload()
{
    m_plugin_path_cache.clear();
    m_kartes.clear(true);
    m_testers.clear(true);

    for (std::vector<void *>::iterator it = m_dls.begin(); it != m_dls.end(); ++it)
    {
        dlclose(*it);
    }

    m_dls.clear();
}
