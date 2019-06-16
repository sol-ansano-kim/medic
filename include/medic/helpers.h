#ifndef MEDIC_HELPERS_INCLUDED
#define MEDIC_HELPERS_INCLUDED

#include "medic/node.h"
#include <maya/MDagPath.h>
#include <maya/MObject.h>

namespace MEDIC
{
    MEDIC_EXPORT bool IsTemplated(MdNode *node);
    MEDIC_EXPORT bool IsTemplated(const MDagPath &path);
    MEDIC_EXPORT bool IsTemplated(const MObject &object);
}

#endif

