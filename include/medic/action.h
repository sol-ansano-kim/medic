#ifndef MEDIC_ACTION_INCLUDED
#define MEDIC_ACTION_INCLUDED


#include "medic/parameter.h"


namespace MEDIC
{
    class MdParameter;

    class MdAction
    {
        public:
            MdAction();
            virtual MdAction *Create();
            virtual ~MdAction();
            virtual void run(MdParameter *p) {};
    };
}


#endif