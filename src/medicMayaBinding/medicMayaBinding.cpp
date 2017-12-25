#include "medicMayaBinding/mayaNodeBinding.h"
#include "medicMayaBinding/mayaReportBinding.h"
#include "medicMayaBinding/mayaTesterBinding.h"
#include "medicMayaBinding/mayaVisitorBinding.h"


using namespace pybind11;


PYBIND11_MODULE(_medicMaya, mod)
{
    bindMayaNode(mod);
    bindMayaReport(mod);
    bindMayaTester(mod);
    bindMayaVisitor(mod);
}
