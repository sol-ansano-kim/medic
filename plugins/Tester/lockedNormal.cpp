#include "medic/pluginapi.h"
#include <maya/MFn.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnMesh.h>
#include <maya/MIntArray.h>

#include <vector>


using namespace MEDIC;


class LockedNormal : public MdTester
{
    std::string Name();
    std::string Description();
    bool Match(MdNode *node);
    bool IsFixable();
    MdReport *test(MdNode *node);
    bool fix(MdReport *report, MdParamContainer *params);
};


std::string LockedNormal::Name()
{
    return "LockedNormal";
}

std::string LockedNormal::Description()
{
    return "vertex(s) which has locked normal";
}

bool LockedNormal::Match(MdNode *node)
{
    return node->object().hasFn(MFn::kMesh);
}

bool LockedNormal::IsFixable()
{
    return true;
}

MdReport *LockedNormal::test(MdNode *node)
{
    MStatus stat;
    MItMeshPolygon it(node->object(), &stat);

    if (stat == MS::kFailure)
    {
        return 0;
    }

    MFnMesh mesh(node->object(), &stat);
    if (stat == MS::kFailure)
    {
        return 0;
    }

    bool result = false;
    std::vector<unsigned int> vertices;
    MFnSingleIndexedComponent comp;
    MObject comp_obj = comp.create(MFn::kMeshVertComponent);

    while (!it.isDone())
    {
        for (unsigned int i = 0; i < it.polygonVertexCount(); i++)
        {
            unsigned vi = it.vertexIndex(i);

            std::vector<unsigned int>::iterator vit = std::find(vertices.begin(), vertices.end(), vi);
            if (vit != vertices.end())
            {
                continue;
            }

            vertices.push_back(vi);

            unsigned int ni = it.normalIndex(i);
            if (mesh.isNormalLocked(ni))
            {
                result = true;
                comp.addElement(vi);
            }
        }

        it.next();
    }

    if (!result)
    {
        return 0;
    }

    return new MdReport(node, comp_obj);
}

bool LockedNormal::fix(MdReport *report, MdParamContainer *params)
{
    MdNode *node = report->node();

    if (node->dg().isFromReferencedFile())
    {
        return false;
    }

    MStatus stat;
    MFnMesh mesh(node->object(), &stat);
    if (stat == MS::kFailure)
    {
        return false;
    }

    MIntArray vertices;
    MFnSingleIndexedComponent comp(report->components());
    comp.getElements(vertices);

    mesh.unlockVertexNormals(vertices);

    return true;
}


MEDIC_PLUGIN_API MdTester *Create()
{
    return new LockedNormal();
}