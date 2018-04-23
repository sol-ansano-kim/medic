import medic
from maya import OpenMaya


class NotFreezed(medic.PyTester):
    Identity = OpenMaya.MMatrix()

    def __init__(self):
        super(NotFreezed, self).__init__()

    def Name(self):
        return "NotFreezed"

    def Description(self):
        return "Not freezed trasnform(s)"

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
    return NotFreezed()
