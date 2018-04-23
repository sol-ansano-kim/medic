import medic
from maya import OpenMaya


class UnFrozenTransforms(medic.PyTester):
    Identity = OpenMaya.MMatrix()

    def __init__(self):
        super(UnFrozenTransforms, self).__init__()

    def Name(self):
        return "UnFrozenTransforms"

    def Description(self):
        return "Check that all transforms are frozen"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kTransform)

    def test(self, node):
        if node.dag().isInstanced():
            return None

        transform = node.dag().transformationMatrix()
        if NotFreezed.Identity == transform:
            return None

        return medic.PyReport(node)


def Create():
    return UnFrozenTransforms()
