#ifndef MEDIC_DEBUG_INCLUDED
#define MEDIC_DEBUG_INCLUDED

#include <string>

#ifdef _DEBUG
#include <iostream>
#endif


namespace MEDIC
{
    void warn(std::string text);
    void debug(std::string text);
    void error(std::string text);
}



#endif // MEDIC_DEBUG_INCLUDED
