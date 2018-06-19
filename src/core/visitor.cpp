#include "medic/visitor.h"


using namespace MEDIC;


MdVisitor::MdVisitor()
    : m_node_collected(false), m_scene(NULL) {}

MdVisitor::~MdVisitor()
{
    reset();
    clearOptions();
}

void MdVisitor::visit(MdKarte *karte)
{
    reset();

    GetNodes(&m_nodes);
    m_node_collected = true;

    MdTesterIterator tester_it = karte->testers();

    while (!tester_it.isDone())
    {
        MdTester *tester = tester_it.next();

        tester->initialize();
        tester->setOptions(getOptions(tester->Name()));

        if (tester->Scope() == MdNodeTester)
        {
            MdNodeIterator node_it = m_nodes.iterator();
            while (!node_it.isDone())
            {
                MdNode *node = node_it.next();
                if (tester->Match(node))
                {
                    MdReport *r = tester->testNode(node);
                    if (r)
                    {
                        addReport(tester, r);
                    }
                }
            }
        }

        else if (tester->Scope() == MdSceneTester && m_scene)
        {
            if (tester->Match(m_scene))
            {
                MdReport *r = tester->testScene(m_scene);
                if (r)
                {
                    addReport(tester, r);
                }
            }
        }

        else if (tester->Scope() == MdAssetTester)
        {
            MdContextIterator asset_it = m_assets.iterator();
            while (!asset_it.isDone())
            {
                MdContext *asset = asset_it.next();
                if (tester->Match(asset))
                {
                    MdReport *r = tester->testAsset(asset);
                    if (r)
                    {
                        addReport(tester, r);
                    }
                }
            }
        }

        tester->finalize();
    }
}

bool MdVisitor::visit(MdKarte *karte, MdTester *tester)
{
    if (!karte->hasTester(tester))
    {
        return false;
    }

    tester->setOptions(getOptions(tester->Name()));

    if (!m_node_collected)
    {
        GetNodes(&m_nodes);
        m_node_collected = true;
    }

    cleanReport(tester);
    tester->initialize();
    tester->setOptions(getOptions(tester->Name()));

    if (tester->Scope() == MdNodeTester)
    {
        MdNodeIterator node_it = m_nodes.iterator();
        while (!node_it.isDone())
        {
            MdNode *node = node_it.next();
            if (tester->Match(node))
            {
                MdReport *r = tester->testNode(node);
                if (r)
                {
                    addReport(tester, r);
                }
            }
        }
    }

    else if (tester->Scope() == MdSceneTester && m_scene)
    {
        if (tester->Match(m_scene))
        {
            MdReport *r = tester->testScene(m_scene);
            if (r)
            {
                addReport(tester, r);
            }
        }
    }

    else if (tester->Scope() == MdAssetTester)
    {
        MdContextIterator asset_it = m_assets.iterator();
        while (!asset_it.isDone())
        {
            MdContext *asset = asset_it.next();
            if (tester->Match(asset))
            {
                MdReport *r = tester->testAsset(asset);
                if (r)
                {
                    addReport(tester, r);
                }
            }
        }
    }

    tester->finalize();

    return true;
}

bool MdVisitor::addReport(MdTester *tester, MdReport *report)
{
    TesterReportsMap::iterator it = m_results.find(tester);
    if (it == m_results.end())
    {
        m_results[tester] = MdReportContainer();   
    }

    return m_results[tester].append(report);
}

void MdVisitor::reset()
{
    m_node_collected = false;

    clearScene();
    clearAssets();

    m_results.clear();
    m_nodes.clear();
}

std::vector<MdTester *> MdVisitor::reportTesters()
{
    std::vector<MdTester *> reported;

    for (TesterReportsMap::iterator it = m_results.begin(); it != m_results.end(); ++it)
    {
        reported.push_back(it->first);
    }

    return reported;
}

MdReportIterator MdVisitor::report(MdTester *tester)
{
    TesterReportsMap::iterator it = m_results.find(tester);
    if (it == m_results.end())
    {
        return MdReportContainer().iterator();
    }

    return it->second.iterator();
}

bool MdVisitor::hasError(MdTester *tester)
{
    TesterReportsMap::iterator it = m_results.find(tester);
    if (it == m_results.end())
    {
        return false;
    }

    return it->second.size();
}

MdNodeIterator MdVisitor::nodes()
{
    if (!m_node_collected)
    {
        GetNodes(&m_nodes);
        m_node_collected = true;
    }

    return m_nodes.iterator();
}

TesterReportsMap &MdVisitor::reportAll()
{
    return m_results;
}

void MdVisitor::clearOptions()
{
    for (OptionMaps::iterator it = m_options.begin(); it != m_options.end(); ++it)
    {
        delete it->second;
    }

    m_options.clear();
}

MdParamContainer* MdVisitor::getOptions(const std::string& name)
{
    OptionMaps::iterator it = m_options.find(name);

    if (it != m_options.end())
    {
        return it->second;
    }

    MdParamContainer* params = new MdParamContainer();
    m_options[name] = params;

    return params;
}

std::vector<std::string> MdVisitor::getOptionKeys()
{
    std::vector<std::string> names;

    for (OptionMaps::iterator it = m_options.begin(); it != m_options.end(); ++it)
    {
        names.push_back(it->first);
    }

    return names;
}

bool MdVisitor::setScene(MdContext *scene)
{
    clearScene();

    m_scene = scene;

    return true;
}

MdContext *MdVisitor::scene()
{
    return m_scene;
}

bool MdVisitor::addAsset(MdContext *asset)
{
    return m_assets.append(asset);
}

MdContextIterator MdVisitor::assets()
{
    return m_assets.iterator();
}

void MdVisitor::cleanReport(MdTester *tester)
{
    TesterReportsMap::iterator it = m_results.find(tester);
    if (it != m_results.end())
    {
        m_results[tester].clear();
        m_results.erase(tester);
    }
}


void MdVisitor::clearScene()
{
    if (m_scene)
    {
        delete m_scene;
    }

    m_scene = NULL;
}

void MdVisitor::clearAssets()
{
    m_assets.clear();
}
