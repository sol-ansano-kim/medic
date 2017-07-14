#include "medic/pluginapi.h"
#include <maya/MFn.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MItMeshEdge.h>
#include <maya/MIntArray.h>
#include <maya/MFnSingleIndexedComponent.h>


using namespace MEDIC;


class NonManifold : public MdTester
{
    std::string Name();
    std::string Description();
    bool Match(MdNode *node);
    MdReport *test(MdNode *node);
};


std::string NonManifold::Name()
{
    return "NonManifold";
}

std::string NonManifold::Description()
{
    return "Non manifold mesh(s) exists";
}

bool NonManifold::Match(MdNode *node)
{
    return node->object().hasFn(MFn::kMesh);
}

MdReport *NonManifold::test(MdNode *node)
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

    while (!it.isDone())
    {
        MIntArray faces;
        it.getConnectedFaces(faces);

        if (faces.length() > 2)
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
    return new NonManifold();
}
