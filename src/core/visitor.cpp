#include "medic/visitor.h"


using namespace MEDIC;


MdVisitor::MdVisitor() {}

MdVisitor::~MdVisitor()
{
    reset();
}

void MdVisitor::visit(MdKarte *k)
{
    reset();

    GetNodes(&m_nodes);

    MdTesterIterator tester_it = k->testers();

    while (!tester_it.isDone())
    {
        MdTester *tester = tester_it.next();

        MdNodeIterator node_it = m_nodes.iterator();
        while (!node_it.isDone())
        {
            MdNode *node = node_it.next();
            if (tester->Match(node))
            {
                MdReport *r = tester->test(node);
                if (r)
                {
                    addReport(tester, r);
                }
            }
        }
    }
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

MdNodeIterator MdVisitor::nodes()
{
    return m_nodes.iterator();
}

TesterReportsMap &MdVisitor::reportAll()
{
    return m_results;
}

