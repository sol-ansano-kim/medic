#include <Python.h>
#include <fstream>
#include <regex>
#include "medic/py.h"
#include "dirent.h"
#include "medic/pluginManager.h"

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG


namespace MEDIC
{
    const std::regex regex_slash_end(".*[/\\\\]$");
    const std::regex regex_py_base("([^/\\\\]+)[.]py$", std::regex_constants::icase);
    const std::regex regex_ext_py(".*[.]py$", std::regex_constants::icase);
    const std::regex regex_ext_karte(".*[.]karte$", std::regex_constants::icase);
    const std::regex regex_key_value("[\"']([^\"']+)[\"']\\s*[:]\\s*[\"']([^\"']+)[\"']\\s*");
    const std::regex regex_key_array("[\"']([^\"']+)[\"']\\s*[:]\\s*(\\[[^[]*\\])\\s*");
    const std::regex regex_sep_array("[\"']([^\"']+)[\"']");
    const std::regex regex_karte_name("Name");
    const std::regex regex_karte_description("Description");
    const std::regex regex_karte_tester("Testers");

    #ifdef _WIN32
        const std::regex regex_path_sep("[;]");
        const std::regex regex_ext_dl(".*[.]dll$", std::regex_constants::icase);
    #else
        const std::regex regex_path_sep("[:]");

        #ifdef __APPLE__
            const std::regex regex_ext_dl(".*[.]bundle$", std::regex_constants::icase);
        #else
            const std::regex regex_ext_dl(".*[.]so$", std::regex_constants::icase);
        #endif // __APPLE__        
    #endif // _WIN32

    typedef MdTester* (*CreateTesterFuncType)();
    void searchFiles(const char* envname, const std::regex& ext, std::vector<std::string> &files);
    bool readKarte(std::string filename, std::string &name, std::string &description, std::vector<std::string> &testers);
}


using namespace MEDIC;


MdPluginManager *MdPluginManager::m_instance = NULL;

MdPluginManager *MdPluginManager::Instance()
{
    if (m_instance == NULL)
    {
        m_instance = new MdPluginManager();
    }

    return m_instance;
}

void MdPluginManager::reloadPlugins()
{
    unloadPlugins();
    loadPlugins();
}

MdPluginManager::MdPluginManager()
{
    loadPlugins();
};

MdPluginManager::~MdPluginManager()
{
    unloadPlugins();
};

void MdPluginManager::searchKartes()
{
    std::vector<std::string> files;
    #ifdef _DEBUG
    std::cout << "[MEDIC] Search kartes...\n";
    #endif // _DEBUG

    searchFiles("MEDIC_KARTE_PATH", regex_ext_karte, files);

    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        std::vector<std::string> patterns;
        std::string name;
        std::string description;

        if (readKarte(*it, name, description, patterns))
        {
            std::map<std::string, MdKarte *>::iterator kit = m_kartes.find(name);
            if (kit != m_kartes.end())
            {
                #ifdef _DEBUG
                std::cout << "[MEDIC] '" << name << "' is already registered. Ignore : " << *it << std::endl;
                #endif // _DEBUG
            }
            else
            {
                #ifdef _DEBUG
                std::cout << "[MEDIC] Register a karte : " << name << std::endl;
                #endif // _DEBUG

                MdKarte *karte = new MdKarte(name, description);
                m_kartes[name] = karte;

                for (std::vector<std::string>::iterator pit = patterns.begin(); pit != patterns.end(); ++pit)
                {
                    #ifdef _DEBUG
                    std::cout << "   add pattern : " << *pit << std::endl;
                    #endif // _DEBUG
                    
                    karte->addPattern(*pit);
                }
            }
        }
    }
}

void MdPluginManager::searchTesters()
{
    std::vector<std::string> dl_files;
    std::vector<std::string> py_files;

    #ifdef _DEBUG
    std::cout << "[MEDIC] Search dl testers...\n";
    #endif // _DEBUG

    searchFiles("MEDIC_TESTER_PATH", regex_ext_dl, dl_files);

    for (std::vector<std::string>::iterator it = dl_files.begin(); it != dl_files.end(); ++it)
    {
        void *dl = dlopen((*it).c_str(), RTLD_LAZY | RTLD_LOCAL);
        if (!dl)
        {
            #ifdef _DEBUG
            std::cout << "[MEDIC] failed to dlopen : " << dlerror() << std::endl;
            #endif

            continue;
        }

        CreateTesterFuncType regFunc = (CreateTesterFuncType) dlsym(dl, "Create");
        if (!regFunc)
        {
            #ifdef _DEBUG
            std::cout << "[MEDIC] failed to dlsym : " << dlerror() << std::endl;
            #endif

            dlclose(dl);
            continue;
        }

        MdTester *tester = regFunc();
        std::string name = tester->Name();

        std::map<std::string, MdTester *>::iterator tit = m_testers.find(name);
        if (tit != m_testers.end())
        {
            #ifdef _DEBUG
            std::cout << "[MEDIC] '" << name << "' is already registered. Ignore : " << *it << std::endl;
            #endif // _DEBUG

            delete tester;
            dlclose(dl);
            continue;
        }

        #ifdef _DEBUG
        std::cout << "[MEDIC] Register a tester : " << name << std::endl;
        #endif // _DEBUG

        m_testers[name] = tester;
        m_dls.push_back(dl);
    }

    #ifdef _DEBUG
    std::cout << "[MEDIC] Search py testers...\n";
    #endif // _DEBUG

    searchFiles("MEDIC_TESTER_PATH", regex_ext_py, py_files);

    PyObject *imp = PyImport_ImportModule("imp");

    if (imp == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        std::cout << "[MEDIC] Failed to load module 'imp'\n";
        #endif // _DEBUG

        return;
    }

    Py_INCREF(imp);

    PyObject *load_source = getPyFunction(imp, "load_source");
    if (load_source == NULL)
    {
        #ifdef _DEBUG
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        std::cout << "[MEDIC] Failed to get function 'load_source'\n";
        #endif // _DEBUG

        return;
    }

    Py_INCREF(load_source);    

    for (std::vector<std::string>::iterator it = py_files.begin(); it != py_files.end(); ++it)
    {
        std::smatch m;
        if (!std::regex_search(*it, m, regex_py_base))
        {
            #ifdef _DEBUG
            std::cout << "[MEDIC] Failed to read python file : " << *it << std::endl;
            #endif // _DEBUG
            continue;
        }

        if (m.size() != 2)
        {
            #ifdef _DEBUG
            std::cout << "[MEDIC] Failed to read python file : " << *it << std::endl;
            #endif // _DEBUG
            continue;
        }

        std::string base = m[1].str();
        PyObject *args = PyTuple_New(2);
        PyObject *arg1 = PyString_FromString(base.c_str());
        PyObject *arg2 = PyString_FromString(it->c_str());
        Py_INCREF(arg1);
        Py_INCREF(arg2);
        PyTuple_SetItem(args, 0, arg1);
        PyTuple_SetItem(args, 1, arg2);

        PyObject *module = PyObject_CallObject(load_source, args);
        if (module == NULL)
        {
            #ifdef _DEBUG
            std::cout << "[MEDIC] Failed to import python file : " << *it << std::endl;
            #endif // _DEBUG

            Py_DECREF(arg1);
            Py_DECREF(arg2);
            Py_DECREF(args);
            continue;
        }

        Py_INCREF(module);

        PyObject *create_func = PyObject_GetAttrString(module, "Create");

        if (create_func == NULL)
        {
            #ifdef _DEBUG
            if (PyErr_Occurred())
            {
                PyErr_Print();
            }
            std::cout << "[MEDIC] Cannot find the 'Create' function : " << *it << std::endl;
            #endif // _DEBUG

            Py_DECREF(arg1);
            Py_DECREF(arg2);
            Py_DECREF(args);
            Py_DECREF(module);
            continue;
        }

        Py_INCREF(create_func);

        if (!PyCallable_Check(create_func))
        {
            #ifdef _DEBUG
            if (PyErr_Occurred())
            {
                PyErr_Print();
            }

            std::cout << "[MEDIC] 'Create' function is not callable : " << *it << std::endl;
            #endif // _DEBUG

            Py_DECREF(arg1);
            Py_DECREF(arg2);
            Py_DECREF(args);
            Py_DECREF(module);
            Py_DECREF(create_func);
            continue;                        
        }

        PyObject *instance = PyObject_CallObject(create_func, PyBlankTuple);
        if (instance == NULL)
        {
            #ifdef _DEBUG
            if (PyErr_Occurred())
            {
                PyErr_Print();
            }
            #endif // _DEBUG

            Py_DECREF(arg1);
            Py_DECREF(arg2);
            Py_DECREF(args);
            Py_DECREF(module);
            Py_DECREF(create_func);
            continue;
        }

        if (MdPyTester::IsValidTester(instance))
        {
            MdTester *tester = new MdPyTester(instance);
            std::string name = tester->Name();

            std::map<std::string, MdTester*>::iterator tit = m_testers.find(name);
            if (tit != m_testers.end())
            {
                #ifdef _DEBUG
                std::cout << "[MEDIC] '" << name << "' is already registered. Ignore : " << *it << std::endl;
                #endif // _DEBUG

                delete tester;
                Py_DECREF(instance);
            }
            else
            {
                #ifdef _DEBUG
                std::cout << "[MEDIC] Register a tester : " << name << std::endl;
                #endif // _DEBUG

                m_testers[name] = tester;
            }
        }

        Py_DECREF(arg1);
        Py_DECREF(arg2);
        Py_DECREF(args);
        Py_DECREF(module);
        Py_DECREF(create_func);
    }

    Py_DECREF(load_source);
    Py_DECREF(imp);
}

void MdPluginManager::loadPlugins()
{
    searchKartes();
    searchTesters();
}

void MdPluginManager::unloadPlugins()
{
    for (std::map<std::string, MdKarte *>::iterator it = m_kartes.begin(); it != m_kartes.end(); ++it)
    {
        delete (it->second);
    }
    m_kartes.clear();

    for (std::map<std::string, MdTester *>::iterator it = m_testers.begin(); it != m_testers.end(); ++it)
    {
        delete (it->second);
    }    
    m_testers.clear();

    for (std::vector<void *>::iterator it = m_dls.begin(); it != m_dls.end(); ++it)
    {
        dlclose(*it);    
    }
    m_dls.clear();    
}


void MEDIC::searchFiles(const char* envname, const std::regex& ext, std::vector<std::string> &files)
{
    struct dirent *entry;
    DIR *dir;
    std::string envpath;
    std::sregex_token_iterator end;

    char *envval = getenv(envname);
    if (envval == NULL)
    {
        return;
    }

    envpath = (std::string)envval;

    for (std::sregex_token_iterator iter(envpath.begin(), envpath.end(), regex_path_sep, -1); iter != end; ++iter)
    {
        if ((dir = opendir(iter->str().c_str())) != NULL)
        {
            while ((entry = readdir(dir)) != NULL)
            {
                if (entry->d_type == DT_REG)
                {
                    std::string fname = entry->d_name;
                    if (std::regex_match(entry->d_name, ext))
                    {
                        std::string finded = iter->str();

                        if (!std::regex_match(finded, regex_slash_end))
                        {
                            finded += '/';
                        }

                        finded += fname;

                        #ifdef _DEBUG
                            std::cout << "[MEDIC] Found '"<< finded << "'\n";
                        #endif // _DEBUG

                        files.push_back(finded);
                    }
                }
            }

            closedir (dir);
        }
    }
}

bool MEDIC::readKarte(std::string filename, std::string &name, std::string &description, std::vector<std::string> &patterns)
{
    bool is_valid_tester = false;
    int length;
    char *cbuffer;
    std::string buffer;
    std::ifstream file(filename);

    if (!file)
    {
        return false;
    }

    file.seekg (0, file.end);
    length = file.tellg();
    file.seekg (0, file.beg);

    cbuffer = new char[length];
    file.read(cbuffer, length);
    buffer = (std::string)cbuffer;

    while (true)
    {
        if (buffer.length() < 1)
        {
            break;
        }

        std::smatch m;
        std::regex_search(buffer, m, regex_key_value);

        if (m.size() != 3)
        {
            break;
        }

        std::string key = m[1].str();
        std::string value = m[2].str();

        if (std::regex_match(key, regex_karte_name))
        {
            name = value;
        }
        else if (std::regex_match(key, regex_karte_description))
        {
            description = value;
        }

        buffer = buffer.substr(m.position() + m.length());
    }

    buffer = (std::string)cbuffer;

    while (true)
    {
        if (buffer.length() < 1)
        {
            break;
        }

        std::smatch m;
        std::regex_search(buffer, m, regex_key_array);

        if (m.size() != 3)
        {
            break;
        }

        std::string key = m[1].str();
        std::string value = m[2].str();

        if (std::regex_match(key, regex_karte_tester))
        {
            std::sregex_token_iterator itend;

            for (std::sregex_token_iterator it = std::sregex_token_iterator(value.begin(), value.end(), regex_sep_array, 1); it != itend; ++it)
            {
                patterns.push_back(it->str());
            }
        }

        buffer = buffer.substr(m.position() + m.length());
    }

    if (name.length() > 0 && description.length() > 0)
    {
        is_valid_tester = true;
    }

    delete[] cbuffer;
    file.close();

    if (!is_valid_tester)
    {
        #ifdef _DEBUG
        std::cout << "[MEDIC] Invalid karte file : " << filename << std::endl;
        #endif // _DEBUG
    }

    return is_valid_tester;
}

