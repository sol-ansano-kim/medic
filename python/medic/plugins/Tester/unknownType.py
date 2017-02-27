from medic.core import testerBase
from maya import OpenMaya


class UnknownType(testerBase.TesterBase):
    Name = "UnknownType"

    def __init__(self):
        super(UnknownType, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kUnknown) or node.object().hasFn(OpenMaya.MFn.kUnknownTransform) or node.object().hasFn(OpenMaya.MFn.kUnknownDag)

    def Test(self, node):
        return (True, None)


Tester = UnknownType


