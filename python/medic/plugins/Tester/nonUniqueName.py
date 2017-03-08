from medic.core import testerBase
from maya import OpenMaya
from medic.core import parameter


class NonUniqueName(testerBase.TesterBase):
    Name = "NonUniqueName"
    Fixable = True

    def __init__(self):
        super(NonUniqueName, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kDagNode)

    def Test(self, node):
        return (not node.dg().hasUniqueName(), None)

    def GetParameters(self):
        return [parameter.Parameter("pattern", "Rename Pattern", parameter.MdString, "")]

    def __setNewName(self, node, base):
        i = 1
        sel = OpenMaya.MSelectionList()
        while (True):
            try:
                sel.add("%s%d" % (base, i))
            except:
                break
            i += 1

        node.dg().setName("%s%d" % (base, i))

    def Fix(self, node, component, parameterParser):
        if node.dg().isFromReferencedFile():
            return False

        rename_target = node

        if not node.dg().hasUniqueName():
            if node.object().hasFn(OpenMaya.MFn.kShape):
                parents = node.dg().parents()
                if parents:
                    parent = parents[0]
                    if not parent.dg().hasUniqueName():
                        rename_target = parent

            pattern = parameterParser.get("pattern")
            base = ""

            if pattern:
                base = pattern

            else:
                base = rename_target.name().split("|")[-1] + "_"

            self.__setNewName(rename_target, base)

        return True


Tester = NonUniqueName
