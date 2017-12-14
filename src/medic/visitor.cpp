#include "medic/visitor.h"


using namespace MEDIC;


MdVisitor::MdVisitor(const MdPluginManager *manager)
    : m_manager(manager), m_karte(NULL) {}

MdVisitor::~MdVisitor()
{
    clearAllReports();
    clearNodes();
}

bool MdVisitor::setKarte(const std::string &name)
{
    const MdKarte *karte = m_manager->getKarte(name);
    if (karte == NULL)
    {
        return false;
    }

    setKarte(karte);

    return true;
}

bool MdVisitor::setKarte(const MdKarte *karte)
{
    clearAllReports();
    m_testers.clear();

    m_karte = karte;

    std::vector<const MdTester *> testers = m_manager->getTesters(karte);
    for (std::vector<const MdTester *>::iterator it = testers.begin(); it != testers.end(); ++it)
    {
        m_testers[(*it)->Name()] = *it;
    }

    return true;
}

const MdTester *MdVisitor::getTester(const std::string &name)
{
    NameTesterMap::iterator it = m_testers.find(name);
    if (it == m_testers.end())
    {
        return NULL;
    }

    return it->second;
}

std::vector<const MdTester *> MdVisitor::getTesters()
{
    std::vector<const MdTester *> testers;
    for (NameTesterMap::iterator it = m_testers.begin(); it != m_testers.end(); ++it)
    {
        testers.push_back(it->second);
    }

    return testers;
}

bool MdVisitor::hasTester(const MdTester *tester)
{
    NameTesterMap::iterator it = m_testers.find(tester->Name());

    return it != m_testers.end();
}

bool MdVisitor::hasTester(const std::string &name)
{
    NameTesterMap::iterator it = m_testers.find(name);

    return it != m_testers.end();
}

void MdVisitor::test(const std::string &name)
{
    const MdTester *tester = getTester(name);
    if (tester == NULL)
    {
        return;
    }

    test(tester);
}

void MdVisitor::test(const MdTester *tester)
{
    if (!hasTester(tester))
    {
        return;
    }

    clearReports(tester);

    for (NodePtrVec::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
    {
        if (!tester->Match(*it))
        {
            continue;
        }

        MdReport *report = tester->test(*it);
        if (report != NULL)
        {
            addReport(tester, report);
        }
    }
}

void MdVisitor::testAll()
{
    clearAllReports();

    for (NameTesterMap::iterator it = m_testers.begin(); it != m_testers.end(); ++it)
    {
        const MdTester *tester = it->second;
        for (NodePtrVec::iterator nit = m_nodes.begin(); nit != m_nodes.end(); ++nit)
        {
            if (!tester->Match(*nit))
            {
                continue;
            }

            MdReport *report = tester->test(*nit);
            if (report != NULL)
            {
                addReport(tester, report);
            }
        }
    }
}

void MdVisitor::reset()
{
    clearAllReports();
    clearNodes();
}

void MdVisitor::collectNodes() {}

void MdVisitor::clearNodes()
{
    for (NodePtrVec::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
    {
        delete (*it);
    }

    m_nodes.clear();
}

void MdVisitor::addReport(const MdTester *tester, MdReport *report)
{
    TesterReportsMap::iterator it = m_reports.find(tester);
    if (it == m_reports.end())
    {
        ReportPtrVec report_vec;
        m_reports[tester] = report_vec;
    }

    return m_reports[tester].push_back(report);
}

void MdVisitor::clearReports(const MdTester *tester)
{
    TesterReportsMap::iterator tit = m_reports.find(tester);
    if (tit == m_reports.end())
    {
        return;
    }

    for (ReportPtrVec::iterator rit = tit->second.begin(); rit != tit->second.end(); ++rit)
    {
        delete (*rit);
    }

    tit->second.clear();
}

void MdVisitor::clearAllReports()
{
    for (TesterReportsMap::iterator tit = m_reports.begin(); tit != m_reports.end(); ++tit)
    {
        for (ReportPtrVec::iterator rit = tit->second.begin(); rit != tit->second.end(); ++rit)
        {
            delete (*rit);
        }

        tit->second.clear();
    }

    m_reports.clear();
}
