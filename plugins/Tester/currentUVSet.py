import medic
from maya import OpenMaya


class CurrentUVSet(medic.PyTester):
    __DefaultSetName = "map1"

    def __init__(self):
        super(CurrentUVSet, self).__init__()

    def Name(self):
        return "CurrentUVSet"

    def Description(self):
        return "The first uvSet is not 'map1'"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def test(self, node):
        mesh = None
        try:
            mesh = OpenMaya.MFnMesh(node.object())
        except:
            return None

        if mesh.currentUVSetName() != CurrentUVSet.__DefaultSetName:
            return medic.PyReport(node)

        return None


def Create():
    return CurrentUVSet()
