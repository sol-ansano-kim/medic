from medic.core import testerBase
from maya import OpenMaya


class NonUniqueName(testerBase.TesterBase):
    Name = "NonUniqueName"

    def __init__(self):
        super(NonUniqueName, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kDagNode)

    def Test(self, node):
        return (not node.dg().hasUniqueName(), None)


Tester = NonUniqueName
