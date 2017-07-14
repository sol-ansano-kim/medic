#include "medic/pluginapi.h"
#include <maya/MFn.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnSingleIndexedComponent.h>


using namespace MEDIC;


class LaminaFace : public MdTester
{
    std::string Name();
    std::string Description();
    bool Match(MdNode *node);
    MdReport *test(MdNode *node);
};


std::string LaminaFace::Name()
{
    return "LaminaFace";
}

std::string LaminaFace::Description()
{
    return "Mesh has lamina face(s)";
}

bool LaminaFace::Match(MdNode *node)
{
    return node->object().hasFn(MFn::kMesh);
}

MdReport *LaminaFace::test(MdNode *node)
{
    MStatus stat;
    MItMeshPolygon it(node->object(), &stat);

    if (stat == MS::kFailure)
    {
        return 0;
    }

    bool result = false;
    MFnSingleIndexedComponent comp;
    MObject comp_obj = comp.create(MFn::kMeshPolygonComponent);

    while (!it.isDone())
    {
        if (it.isLamina())
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
    return new LaminaFace();
}