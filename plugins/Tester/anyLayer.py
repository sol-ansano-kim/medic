import medic
from maya import OpenMaya
from maya import cmds


class AnyLayer(medic.PyTester):
    def __init__(self):
        super(AnyLayer, self).__init__()

    def Name(self):
        return "AnyLayer"

    def Description(self):
        return "Check if any render, anim or display layer exists"

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

        cmds.delete(node.name())

        return True


def Create():
    return AnyLayer()
