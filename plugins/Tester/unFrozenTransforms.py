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
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def test(self, node):
        if node.dag().isInstanced():
            return None

        iden = OpenMaya.MMatrix()

        for p in node.parents():
            transform = p.dag().transformationMatrix()
            if not UnFrozenTransforms.Identity == transform:
                return medic.PyReport(node)

        return None


def Create():
    return UnFrozenTransforms()
