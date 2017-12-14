#ifndef MEDIC_REPORT_INCLUDED
#define MEDIC_REPORT_INCLUDED


namespace MEDIC
{
    class MdNode;

    class MdReport
    {
        public:
            MdReport(const MdNode *node);
            virtual ~MdReport();
            const MdNode *node() const;

        protected:
            const MdNode *m_node;
    };
}


#endif // MEDIC_REPORT_INCLUDED