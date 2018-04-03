#include "medic/pluginapi.h"
#include <cfloat>
#include <maya/MFn.h>
#include <maya/MItMeshEdge.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MFnSingleIndexedComponent.h>


using namespace MEDIC;


class ZeroLengthEdge : public MdTester
{
    std::string Name();
    std::string Description();
    bool Match(MdNode *node);
    MdReport *test(MdNode *node);
};


std::string ZeroLengthEdge::Name()
{
    return "ZeroLengthEdge";
}

std::string ZeroLengthEdge::Description()
{
    return "Check if any edge length is zero";
}

bool ZeroLengthEdge::Match(MdNode *node)
{
    return node->object().hasFn(MFn::kMesh);
}

MdReport *ZeroLengthEdge::test(MdNode *node)
{
    MStatus stat;
    MItMeshEdge it(node->object(), &stat);
    if (stat == MS::kFailure)
    {
        return 0;
    }

    bool result = false;

    MFnSingleIndexedComponent comp;
    MObject comp_obj = comp.create(MFn::kMeshEdgeComponent);

    while(!it.isDone())
    {
        double length;
        it.getLength(length);
        if (DBL_EPSILON > length)
        {
            result = true;
            comp.addElement(it.index());
        }

        it.next();
    }

    if (!result)
    {
        return 0;
    }

    return new MdReport(node, comp_obj);
}


MEDIC_PLUGIN_API MdTester *Create()
{
    return new ZeroLengthEdge();
}
