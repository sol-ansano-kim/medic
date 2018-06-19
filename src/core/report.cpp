#include "medic/report.h"


using namespace MEDIC;


MdReport::MdReport(MdNode *node, MObject &compObj)
    : m_node(node), m_context(NULL), m_comp(compObj), m_has_components(true) {}

MdReport::MdReport(MdNode *node)
    : m_node(node), m_context(NULL), m_has_components(false) {}

MdReport::MdReport(MdContext *context)
    : m_node(NULL), m_context(context), m_has_components(false) {}

MdReport::~MdReport() {};

void MdReport::addSelection() const
{
    if (m_node == NULL)
    {
        return;
    }

    selection_list.clear();

    if (m_node->isDag())
    {
        if (m_has_components)
        {
            selection_list.add(m_node->getPath(), m_comp);
        }
        else
        {
            selection_list.add(m_node->getPath());
        }
    }
    else
    {
        selection_list.add(m_node->object());
    }

    MGlobal::setActiveSelectionList(selection_list, MGlobal::kAddToList);
}

MdNode *MdReport::node()
{
    return m_node;
}

MdContext *MdReport::context()
{
    return m_context;
}

MObject &MdReport::components()
{
    return m_comp;
}

bool MdReport::hasComponents() const
{
    return m_has_components;
}


MdReportContainer::MdReportContainer() {}

MdReportContainer::~MdReportContainer()
{
    clear();
}

bool MdReportContainer::append(MdReport *r)
{
    if (std::find(m_reports.begin(), m_reports.end(), r) == m_reports.end())
    {
        m_reports.push_back(r);
        return true;
    }

    return false;
}

bool MdReportContainer::remove(MdReport *r)
{
    ReportPtrVec::iterator it = std::find(m_reports.begin(), m_reports.end(), r);
    if (it != m_reports.end())
    {
        m_reports.erase(it);
        delete (*it);
        return true;
    }

    return false;
}

void MdReportContainer::clear()
{
    for (ReportPtrVec::iterator it = m_reports.begin(); it != m_reports.end(); ++it)
    {
        delete (*it);
    }

    m_reports.clear();
}

size_t MdReportContainer::size() const
{
    return m_reports.size();
}

MdReportIterator MdReportContainer::iterator()
{
    return MdReportIterator(this);
}


MdReportIterator::MdReportIterator() {}

MdReportIterator::MdReportIterator(MdReportContainer *container)
{
    m_container = container;
    m_cur = container->m_reports.begin();
}

MdReportIterator::~MdReportIterator() {}

MdReport *MdReportIterator::next()
{
    return *(m_cur++);
}

bool MdReportIterator::isDone()
{
    if (m_container == 0)
    {
        return true;
    }

    return (m_cur == m_container->m_reports.end());
}
