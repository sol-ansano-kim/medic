#ifndef MEDIC_RESULT_INCLUDED
#define MEDIC_RESULT_INCLUDED


#include <maya/MObject.h>
#include <vector>
#include "medic/node.h"
#include "medic/context.h"
#include "medic/platform.h"
#include "maya/MSelectionList.h"
#include "maya/MGlobal.h"


namespace MEDIC
{
    class MdReport;
    class MdReportContainer;
    class MdReportIterator;
    static MSelectionList selection_list;

    class MdReport
    {
        public:
            MEDIC_EXPORT MdReport(MdNode *node, MObject &compObj);
            MEDIC_EXPORT MdReport(MdNode *node);
            MEDIC_EXPORT MdReport(MdContext* context);
            MEDIC_EXPORT ~MdReport();
            MEDIC_EXPORT void addSelection() const;
            MEDIC_EXPORT MdNode *node();
            MEDIC_EXPORT MdContext *context();
            MEDIC_EXPORT MObject &components();
            bool hasComponents() const;

        private:
            MdNode *m_node;
            MdContext *m_context;
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
            MEDIC_EXPORT MdReportIterator();
            MdReportIterator(MdReportContainer *container);
            MEDIC_EXPORT ~MdReportIterator();
            MEDIC_EXPORT MdReport *next();
            MEDIC_EXPORT bool isDone();

        private:
            MdReportContainer *m_container;
            ReportPtrVec::iterator m_cur;
    };
}


#endif