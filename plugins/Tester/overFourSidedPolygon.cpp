#include "medic/pluginapi.h"
#include <maya/MFn.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnSingleIndexedComponent.h>


using namespace MEDIC;


class OverFourSidedPolygon : public MdTester
{
    std::string Name();
    std::string Description();
    bool Match(MdNode *node);
    MdReport *test(MdNode *node);
};


std::string OverFourSidedPolygon::Name()
{
    return "OverFourSidedPolygon";
}

std::string OverFourSidedPolygon::Description()
{
    return "5 or more faced polygon(s)";
}

bool OverFourSidedPolygon::Match(MdNode *node)
{
    return node->object().hasFn(MFn::kMesh);
}

MdReport *OverFourSidedPolygon::test(MdNode *node)
{
    MStatus stat;
    MItMeshPolygon it(node->object(), &stat);

    if (stat == MS::kFailure)
    {
        return 0;
    }

    MFnSingleIndexedComponent comp;
    MObject comp_obj = comp.create(MFn::kMeshPolygonComponent);

    bool result = false;

    while (!it.isDone())
    {
        if (it.polygonVertexCount() > 4)
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
    return new OverFourSidedPolygon();
}
