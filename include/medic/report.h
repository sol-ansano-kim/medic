#ifndef MEDIC_RESULT_INCLUDED
#define MEDIC_RESULT_INCLUDED


#include <maya/MObject.h>
#include <vector>
#include "medic/node.h"
#include "maya/MSelectionList.h"


namespace MEDIC
{
    class MdReport;
    class MdReportContainer;
    class MdReportIterator;

    class MdReport
    {
        public:
            MdReport(MdNode *node, MObject &compObj);
            MdReport(MdNode *node);
            ~MdReport();
            void addSelection(MSelectionList &selection) const;
            MdNode *node();
            MObject &components();
            bool hasComponents();

        private:
            MdNode *m_node;
            MObject m_comp;
            bool m_has_components;
    };

    typedef std::vector<MdReport *> ReportPtrVec;

    class MdReportContainer
    {
        friend class MdReportIterator;
        public:
            MdReportContainer();
            ~MdReportContainer();
            bool append(MdReport *n);
            bool remove(MdReport *n);
            void clear();
            size_t size() const;
            MdReportIterator iterator();

        private:
            ReportPtrVec m_reports;
            
    };

    class MdReportIterator
    {
        public:
            MdReportIterator();
            MdReportIterator(MdReportContainer *container);
            ~MdReportIterator();
            MdReport *next();
            bool isDone();

        private:
            MdReportContainer *m_container;
            ReportPtrVec::iterator m_cur;
    };
}


#endif