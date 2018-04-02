#include "medic/pluginapi.h"
#include <maya/MFn.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MItMeshFaceVertex.h>
#include <maya/MFnMesh.h>
#include <maya/MStringArray.h>
#include <maya/MFnSingleIndexedComponent.h>


using namespace MEDIC;


class IncompleteUV : public MdTester
{
    std::string Name();
    std::string Description();
    bool Match(MdNode *node);
    MdReport *test(MdNode *node);
};


std::string IncompleteUV::Name()
{
    return "IncompleteUV";
}

std::string IncompleteUV::Description()
{
    return "Vertex has no UVs";
}

bool IncompleteUV::Match(MdNode *node)
{
    return node->object().hasFn(MFn::kMesh);
}

MdReport *IncompleteUV::test(MdNode *node)
{
    MStatus stat;
    bool error = false;
    MStringArray uvsets;
    MFnSingleIndexedComponent comp;
    MObject comp_obj = comp.create(MFn::kMeshVertComponent);

    MItMeshFaceVertex it(node->object(), &stat);

    if (stat == MS::kFailure)
    {
        return 0;
    }

    MFnMesh mesh(node->object(), &stat);
    if (stat == MS::kFailure)
    {
        return 0;
    }

    mesh.getUVSetNames(uvsets);

    if (uvsets.length() < 1)
    {
        return new MdReport(node);
    }

    while (!it.isDone())
    {
        for (unsigned int i = 0; i < uvsets.length(); ++i)
        {
            if (!it.hasUVs(uvsets[i]))
            {
                error = true;
                comp.addElement(it.vertId());
                break;
            }
        }

        it.next();
    }

    if (!error)
    {
        return 0;
    }

    return new MdReport(node, comp_obj);
}


MEDIC_PLUGIN_API MdTester *Create()
{
    return new IncompleteUV();
}
