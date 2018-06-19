import medic
from maya import OpenMaya


class NonUniqueName(medic.PyTester):
    def __init__(self):
        super(NonUniqueName, self).__init__()

    def Name(self):
        return "NonUniqueName"

    def Description(self):
        return "Non unique name(s) exists"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kDagNode)

    def IsFixable(self):
        return True

    def GetParameters(self):
        con = medic.ParamContainer.Create()
        p = medic.Parameter.Create("pattern", "Rename Pattern", medic.Types.String, "")
        con.append(p)
        return con

    def testNode(self, node):
        if node.dg().hasUniqueName():
            return None

        return medic.PyReport(node)

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

    def fix(self, report, params):
        node = report.node()
        if node.dg().isFromReferencedFile():
            return False

        rename_target = node

        if not node.dg().hasUniqueName():
            if node.object().hasFn(OpenMaya.MFn.kShape) and node.isDag():
                parents = node.parents()
                if parents:
                    parent = parents[0]
                    if not parent.dg().hasUniqueName():
                        rename_target = parent

            pattern = params.get("pattern")
            base = ""

            if pattern:
                base = pattern

            else:
                base = rename_target.name().split("|")[-1] + "_"

            self.__setNewName(rename_target, base)

        return True


def Create():
    return NonUniqueName()
