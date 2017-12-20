#include "medicBinding/parameterBinding.h"
#include "medicBinding/karteBinding.h"
#include "medicBinding/nodeBinding.h"
#include "medicBinding/reportBinding.h"
#include "medicBinding/testerBinding.h"
#include "medicBinding/pluginManagerBinding.h"
#include "medicBinding/visitorBinding.h"
#include "medicMayaBinding/mayaNodeBinding.h"
#include "medicMayaBinding/mayaReportBinding.h"
#include "medicMayaBinding/mayaTesterBinding.h"


using namespace pybind11;


PYBIND11_MODULE(_medicMaya, mod)
{
    bindParameter(mod);
    bindNode(mod);
    bindReport(mod);
    bindKarte(mod);
    bindTester(mod);
    bindPluginManager(mod);
    bindVisitor(mod);

    bindMayaNode(mod);
    bindMayaReport(mod);
    bindMayaTester(mod);
}
