#include "medic/pluginapi.h"
#include "maya/MFn.h"
#include "maya/MFnDependencyNode.h"


using namespace MEDIC;


class NonUniqueName : public MdTester
{
    public:
        NonUniqueName();
        ~NonUniqueName();
        std::string Name();
        std::string Description();
        bool Match(MdNode *node);
        bool isFixable() const;
        MdReport *test(MdNode *node) const;
        MdParamContainer *GetParameters() const;
        bool Fix(MdReport *report) const;
};


NonUniqueName::NonUniqueName() {}

NonUniqueName::~NonUniqueName() {}

std::string NonUniqueName::Name()
{
    return "NonUniqueName";
}

std::string NonUniqueName::Description()
{
    return "Non unique name(s) exists";
}

bool NonUniqueName::Match(MdNode *node)
{
    return node->object().hasFn(MFn::kDagNode);
}

bool NonUniqueName::isFixable() const
{
    return true;
}

MdReport *NonUniqueName::test(MdNode *node) const
{
    MFnDependencyNode dg(node->object());
    if (dg.hasUniqueName())
    {
        return 0;
    }

    return new MdReport(node);
}

MdParamContainer *NonUniqueName::GetParameters() const
{
    MdParamContainer *params = new MdParamContainer();
    MdParameter *pattern = new MdParameter("pattern", "Rename Pattern", MdString, "");
    params->append(pattern);

    return params;
}

bool NonUniqueName::Fix(MdReport *report) const
{
    return true;
}


MEDIC_PLUGIN_API MdTester *Create()
{
    return new NonUniqueName();
}
