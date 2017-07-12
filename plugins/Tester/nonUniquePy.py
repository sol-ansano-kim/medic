import medic
from maya import OpenMaya


class NonUniquePy(medic.PyTester):
    def __init__(self):
        super(NonUniquePy, self).__init__()

    def Name(self):
        return "NonUniquePy"

    def Description(self):
        return ""

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kDagNode)

    def IsFixable(self):
        return True

    def GetParameters(self):
        con = medic.ParamContainer()
        p = medic.Parameter.Create("pattern", "Rename Pattern", medic.Types.String, "")
        con.append(p)
        return con

    def test(self, node):
        if node.dg().hasUniqueName():
            return None

        return medic.PyReport(node)

    def fix(self, report, params):
        return True


def Create():
    return NonUniquePy()
