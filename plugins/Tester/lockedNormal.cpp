#include "medic/pluginapi.h"
#include <set>
#include <maya/MFn.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnMesh.h>
#include <maya/MIntArray.h>


using namespace MEDIC;


class LockedNormal : public MdTester
{
    std::string Name();
    std::string Description();
    bool Match(MdNode *node);
    bool IsFixable();
    MdReport *testNode(MdNode *node);
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

MdReport *LockedNormal::testNode(MdNode *node)
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
    MFnSingleIndexedComponent comp;
    MObject comp_obj = comp.create(MFn::kMeshVertComponent);
    std::set<unsigned int> vertices;
    std::set<unsigned int>::iterator vit;

    while (!it.isDone())
    {
        for (unsigned int i = 0; i < it.polygonVertexCount(); i++)
        {
            unsigned int vi = it.vertexIndex(i);

            vit = vertices.find(vi);

            if (vit != vertices.end())
            {
                continue;
            }

            vertices.insert(vi);

            unsigned int ni = it.normalIndex(i);
            if (mesh.isNormalLocked(ni))
            {
                result = true;
                comp.addElement(it.vertexIndex(i));
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