import medic
from maya import OpenMaya


class InstanceNode(medic.PyTester):
    def __init__(self):
        super(InstanceNode, self).__init__()

    def Name(self):
        return "InstanceNode"

    def Description(self):
        return "Instance node(s) exists"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kDagNode)

    def test(self, node):
        if node.dag().isInstanced():
            return medic.PyReport(node)

        return None


def Create():
    return InstanceNode()
