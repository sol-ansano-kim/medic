#ifndef MEDIC_ACTION_INCLUDED
#define MEDIC_ACTION_INCLUDED


namespace MEDIC
{
    class MdParameter;

    class MdAction
    {
        public:
            MdAction();
            virtual MdAction *Create() const;
            virtual ~MdAction();
            virtual void run(MdParameter *p) {};
    };
}


#endif