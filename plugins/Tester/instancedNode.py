import medic
from maya import OpenMaya


class InstancedNode(medic.PyTester):
    def __init__(self):
        super(InstancedNode, self).__init__()

    def Name(self):
        return "InstancedNode"

    def Description(self):
        return "Instanced node(s) exists"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kDagNode)

    def test(self, node):
        if node.dag().isInstanced():
            return medic.PyReport(node)

        return None


def Create():
    return InstancedNode()
