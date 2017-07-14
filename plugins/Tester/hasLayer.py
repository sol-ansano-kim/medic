import medic
from maya import OpenMaya


class HasLayer(medic.PyTester):
    def __init__(self):
        super(HasLayer, self).__init__()

    def Name(self):
        return "HasLayer"

    def Description(self):
        return "layer(s) exists"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kDisplayLayer) or node.object().hasFn(OpenMaya.MFn.kAnimLayer) or node.object().hasFn(OpenMaya.MFn.kRenderLayer)

    def test(self, node):
        if node.name() not in ["BaseAnimation", "defaultRenderLayer", "defaultLayer"]:
            return medic.PyReport(node)

        return None

    def IsFixable(self):
        return True

    def fix(self, report, params):
        node = report.node()
        if node.dg().isFromReferencedFile():
            return False

        if node.dg().isLocked():
            node.dg().setLocked(False)

        mod = OpenMaya.MDGModifier()
        mod.deleteNode(node.object())
        mod.doIt()


def Create():
    return HasLayer()
