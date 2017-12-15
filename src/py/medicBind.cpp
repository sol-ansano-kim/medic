#include "medic/py/parameterBind.h"
#include "medic/py/karteBind.h"
#include "medic/py/nodeBind.h"
#include "medic/py/reportBind.h"
#include "medic/py/testerBind.h"
#include "medic/py/pluginManagerBind.h"
#include "medic/py/visitorBind.h"


using namespace pybind11;


PYBIND11_MODULE(_medic, mod)
{
    bindParameter(mod);
    bindNode(mod);
    bindReport(mod);
    bindKarte(mod);
    bindTester(mod);
    bindPluginManager(mod);
    bindVisitor(mod);
}
