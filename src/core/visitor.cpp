#include "medic/visitor.h"
#include <maya/MGlobal.h>


using namespace MEDIC;


bool MdVisitor::warned_visit_karter = false;
bool MdVisitor::warned_visit_tester = false;


MdVisitor::MdVisitor()
    : m_node_collected(false), m_collect_in_selection(false), m_scene(NULL) {}

MdVisitor::~MdVisitor()
{
    reset();
    clearOptions();
}

void MdVisitor::visit(MdKarte *karte)
{
    if (!warned_visit_karter)
    {
        MGlobal::displayWarning("MdVisitor::visit(MdKarte *karte) is deprecated. please use MdVisitor::visitKarte(MdKarte *karte) instead");
        warned_visit_karter = true;
    }

    visitKarte(karte);
}

void MdVisitor::visitKarte(MdKarte *karte)
{
    reset();

    collectNodes();
    m_node_collected = true;

    MdTesterIterator tester_it = karte->testers();

    while (!tester_it.isDone())
    {
        MdTester *tester = tester_it.next();
        visit(karte, tester);
    }
}

bool MdVisitor::visit(MdKarte *karte, MdTester *tester)
{
    if (!warned_visit_tester)
    {
        MGlobal::displayWarning("MdVisitor::visit(MdKarte *karte, MdTester *tester) is deprecated. please use MdVisitor::visitTester(MdTester *tester) instead");
        warned_visit_tester = true;
    }

    if (!karte->hasTester(tester))
    {
        return false;
    }

    visitTester(tester);

    return true;
}

void MdVisitor::visitTester(MdTester *tester)
{
    tester->setOptions(getOptions(tester->Name()));

    if (!m_node_collected)
    {
        collectNodes();
        m_node_collected = true;
    }

    cleanReport(tester);
    initializeTester(tester);

    if (tester->Scope() == MdNodeTester)
    {
        MdNodeIterator node_it = m_nodes.iterator();
        while (!node_it.isDone())
        {
            visitNode(tester, node_it.next());
        }
    }

    else if (tester->Scope() == MdSceneTester && m_scene)
    {
        if (tester->Match(m_scene))
        {
            visitContext(tester, m_scene);
        }
    }

    else if (tester->Scope() == MdAssetTester)
    {
        MdContextIterator asset_it = m_assets.iterator();
        while (!asset_it.isDone())
        {
            visitContext(tester, asset_it.next());
        }
    }

    finalizeTester(tester);
}

void MdVisitor::visitNode(MdTester *tester, MdNode *node)
{
    if (tester->Match(node))
    {
        MdReport *r = tester->test(node);
        if (r)
        {
            addReport(tester, r);
        }
    }
}

void MdVisitor::visitContext(MdTester *tester, MdContext *context)
{
    if (tester->Match(context))
    {
        MdReport *r = tester->test(context);
        if (r)
        {
            addReport(tester, r);
        }
    }
}

bool MdVisitor::canVisit(MdKarte *karte, MdTester *tester, MdNode *node, MdContext *context)
{
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

    return it->second.size() != 0;
}

MdNodeIterator MdVisitor::nodes()
{
    if (!m_node_collected)
    {
        collectNodes();
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


bool MdVisitor::selectionOnly() const
{
    return m_collect_in_selection;
}

void MdVisitor::initializeTester(MdTester *tester)
{
    tester->initialize();
    tester->setOptions(getOptions(tester->Name()));
}

void MdVisitor::finalizeTester(MdTester *tester)
{
    tester->finalize();
}

void MdVisitor::setSelectionOnly(bool v)
{
    m_collect_in_selection = v;
}

void MdVisitor::collectNodes()
{
    if (m_collect_in_selection)
    {
        GetNodesInSelection(&m_nodes);
    }
    else
    {
        GetNodes(&m_nodes);
    }
}