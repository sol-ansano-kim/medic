#include "medic/visitor.h"


using namespace MEDIC;


MdVisitor::MdVisitor(const MdKarte *karte) : m_karte(karte) {}

MdVisitor::~MdVisitor()
{
    clearAllReports();
    clearNodes();
}

const MdTester *MdVisitor::tester(const std::string &name)
{
    NameTesterMap::iterator it = m_testers.find(name);
    if (it == m_testers.end())
    {
        return NULL;
    }

    return it->second;
}

std::vector<const MdTester *> MdVisitor::testers()
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
    if (it == m_testers.end())
    {
        return false;
    }

    return true;
}

void MdVisitor::test(const std::string &name) {}

void MdVisitor::testAll() {}

void MdVisitor::reset()
{
    clearAllReports();
    clearNodes();
}

void MdVisitor::collectNodes() {}

void MdVisitor::clearNodes() {}

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
