#include "medicMayaBinding/mayaNodeBinding.h"


using namespace pybind11;


PYBIND11_MODULE(_medicMaya, mod)
{
    bindMayaNode(mod);
}
