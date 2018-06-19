import medic
from maya import OpenMaya


class ReferencedNode(medic.PyTester):
    def __init__(self):
        super(ReferencedNode, self).__init__()

    def Name(self):
        return "ReferencedNode"

    def Description(self):
        return "Refrenced node(s) exists"

    def Match(self, node):
        return True

    def test(self, node):
        if node.dg().isFromReferencedFile():
            return medic.PyReport(node)

        return None


def Create():
    return ReferencedNode()
