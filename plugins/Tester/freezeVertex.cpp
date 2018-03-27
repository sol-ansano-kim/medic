#include "medic/pluginapi.h"
#include <set>
#include <maya/MFn.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <cmath>


using namespace MEDIC;


#define FreezeVertexEpsilon 0.000001


class FreezeVertex : public MdTester
{
    std::string Name();
    std::string Description();
    bool Match(MdNode *node);
    bool IsFixable();
    MdReport *test(MdNode *node);
    bool fix(MdReport *report, MdParamContainer *params);
};


std::string FreezeVertex::Name()
{
    return "FreezeVertex";
}

std::string FreezeVertex::Description()
{
    return "Not Frozen Verticies";
}

bool FreezeVertex::Match(MdNode *node)
{
    return node->object().hasFn(MFn::kMesh);
}

bool FreezeVertex::IsFixable()
{
    return true;
}

MdReport *FreezeVertex::test(MdNode *node)
{
    MPlug plug = node->dg().findPlug("pnts");

    for (unsigned int i = 0; i < plug.numElements(); ++i)
    {
        MPlug elm = plug.elementByPhysicalIndex(i);

        for (unsigned int j = 0; j < 3; ++j)
        {
            if (std::fabs(elm.child(j).asDouble()) > FreezeVertexEpsilon)
            {
                return new MdReport(node);
            }
        }
    }

    return 0;
}

bool FreezeVertex::fix(MdReport *report, MdParamContainer *params)
{
    MString cmd("polyMoveVertex -ch 0 ");
    cmd += report->node()->name().c_str();

    if (MS::kFailure == MGlobal::executeCommand(cmd))
    {
        return false;
    }

    return true;
}


MEDIC_PLUGIN_API MdTester *Create()
{
    return new FreezeVertex();
}
