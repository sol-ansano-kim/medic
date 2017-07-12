import medic
from maya import OpenMaya


class Referenced(medic.PyTester):
    def __init__(self):
        super(Referenced, self).__init__()

    def Name(self):
        return "Referenced"

    def Description(self):
        return "Refrenced node(s) exists"

    def Match(self, node):
        return True

    def test(self, node):
        if node.dg().isFromReferencedFile():
            return medic.PyReport(node)

        return None


def Create():
    return Referenced()
