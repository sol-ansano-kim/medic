#ifndef MEDIC_REPORT_INCLUDED
#define MEDIC_REPORT_INCLUDED


namespace MEDIC
{
    class MdNode;

    class MdReport
    {
        public:
            MdReport(MdNode *node);
            virtual ~MdReport();
            MdNode *node() const;
            virtual void addSelection() const = 0;

        private:
            MdNode *m_node;
    };
}


#endif