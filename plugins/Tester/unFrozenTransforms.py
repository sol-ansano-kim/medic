import medic
from maya import OpenMaya
from maya import cmds


class UnFrozenTransforms(medic.PyTester):
    Identity = OpenMaya.MMatrix()

    def __init__(self):
        super(UnFrozenTransforms, self).__init__()

    def Name(self):
        return "UnFrozenTransforms"

    def Description(self):
        return "Check that all transforms are frozen"

    def Dependencies(self):
        return ["ConstructionHistory"]

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kTransform)

    def IsFixable(self):
        return True

    def test(self, node):
        if node.dag().isInstanced():
            return None

        transform = node.dag().transformationMatrix()
        if NotFreezed.Identity == transform:
            return None

        return medic.PyReport(node)

    def fix(self, report, params):
        cmds.makeIdentity(report.node().name(), apply=True, t=True, r=True, s=True)

        return True


def Create():
    return UnFrozenTransforms()
