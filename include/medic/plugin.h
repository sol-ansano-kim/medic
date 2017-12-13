#ifndef MEDIC_PLUGIN_INCLUDED
#define MEDIC_PLUGIN_INCLUDED


#include "medic/platform.h"
#include "medic/node.h"
#include "medic/tester.h"
#include "medic/parameter.h"
#include "medic/report.h"


#define MEDIC_TESTER extern "C" MEDIC_EXPORT MdTester *Create()


#endif // MEDIC_PLUGIN_INCLUDED