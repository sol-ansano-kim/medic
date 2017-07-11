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
        bool IsFixable() const;
        MdParamContainer *GetParameters() const;
        MdReport *test(MdNode *node) const;
        bool fix(MdReport *report, MdParamContainer *params) const;
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

bool NonUniqueName::IsFixable() const
{
    return true;
}

MdParamContainer *NonUniqueName::GetParameters() const
{
    MdParamContainer *params = new MdParamContainer();
    MdParameter *pattern = new MdParameter("pattern", "Rename Pattern", MdString, "");
    params->append(pattern);

    return params;
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

bool NonUniqueName::fix(MdReport *report, MdParamContainer *params) const
{
    return true;
}


MEDIC_PLUGIN_API MdTester *Create()
{
    return new NonUniqueName();
}
