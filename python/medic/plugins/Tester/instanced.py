from medic.core import testerBase
from maya import OpenMaya


class InstanceNode(testerBase.TesterBase):
    Name = "InstanceNode"

    def __init__(self):
        super(InstanceNode, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kDagNode)

    def Test(self, node):
        return (node.dag().isInstanced(), None)


Tester = InstanceNode
