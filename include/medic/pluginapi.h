#ifdef _WIN32
    #define MEDIC_PLUGIN_API extern "C" __declspec(dllexport)
#else
    #define MEDIC_PLUGIN_API extern "C" __attribute__ ((visibility ("default")))
#endif

#include "medic/tester.h"
#include "medic/node.h"
#include "medic/parameter.h"
#include "medic/report.h"
