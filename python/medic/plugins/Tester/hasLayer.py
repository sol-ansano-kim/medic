from medic.core import testerBase
from maya import OpenMaya


class HasLayer(testerBase.TesterBase):
    Name = "HasLayer"
    Fixable = True

    def __init__(self):
        super(HasLayer, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kDisplayLayer) or node.object().hasFn(OpenMaya.MFn.kAnimLayer) or node.object().hasFn(OpenMaya.MFn.kRenderLayer)

    def Test(self, node):
        return (node.name() not in ["BaseAnimation", "defaultRenderLayer", "defaultLayer"], None)

    def GetParameters(self):
        return []

    def Fix(self, node, component, parameterParser):
        if node.dg().isFromReferencedFile():
            return False

        if node.dg().isLocked():
            node.dg().setLocked(False)

        mod = OpenMaya.MDGModifier()
        mod.deleteNode(node.object())
        mod.doIt()
        return True


Tester = HasLayer
