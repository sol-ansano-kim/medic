from medic.core import testerBase
from maya import OpenMaya


class UnknownType(testerBase.TesterBase):
    Name = "UnknownType"
    Fixable = True

    def __init__(self):
        super(UnknownType, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kUnknown) or node.object().hasFn(OpenMaya.MFn.kUnknownTransform) or node.object().hasFn(OpenMaya.MFn.kUnknownDag)

    def Test(self, node):
        return (True, None)

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


Tester = UnknownType


