#include "medic/pluginapi.h"
#include <maya/MFn.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnSingleIndexedComponent.h>


using namespace MEDIC;


class NGon : public MdTester
{
    std::string Name();
    std::string Description();
    bool Match(MdNode *node);
    MdReport *test(MdNode *node);
};


std::string NGon::Name()
{
    return "NGon";
}

std::string NGon::Description()
{
    return "Mesh has an Ngon (5 =< sided polygon)";
}

bool NGon::Match(MdNode *node)
{
    return node->object().hasFn(MFn::kMesh);
}

MdReport *NGon::test(MdNode *node)
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
    return new NGon();
}
