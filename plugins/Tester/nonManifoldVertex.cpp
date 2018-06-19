#include "medic/pluginapi.h"
#include <maya/MFn.h>
#include <maya/MItMeshVertex.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MIntArray.h>
#include <maya/MFnSingleIndexedComponent.h>


using namespace MEDIC;


class NonManifoldVertex : public MdTester
{
    std::string Name();
    std::string Description();
    bool Match(MdNode *node);
    MdReport *testNode(MdNode *node);
};


std::string NonManifoldVertex::Name()
{
    return "NonManifoldVertex";
}

std::string NonManifoldVertex::Description()
{
    return "Non manifold vertex(s) exists";
}

bool NonManifoldVertex::Match(MdNode *node)
{
    return node->object().hasFn(MFn::kMesh);
}

MdReport *NonManifoldVertex::testNode(MdNode *node)
{
    MItMeshVertex itvtx(node->getPath());
    MFnSingleIndexedComponent comp;
    MObject comp_obj = comp.create(MFn::kMeshVertComponent);
    bool result = false;

    while (!itvtx.isDone())
    {
        MIntArray faces;
        MIntArray edges;
        itvtx.getConnectedFaces(faces);
        itvtx.getConnectedEdges(edges);

        if ((edges.length() - faces.length()) > 1)
        {
            result = true;
            comp.addElement(itvtx.index());
        }

        itvtx.next();
    }

    if (!result)
    {
        return 0;
    }

    return new MdReport(node, comp_obj);
}


MEDIC_PLUGIN_API MdTester *Create()
{
    return new NonManifoldVertex();
}
