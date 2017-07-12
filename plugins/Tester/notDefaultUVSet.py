import medic
from maya import OpenMaya


class NotDefaultUVSet(medic.PyTester):
    __DefaultSetName = "map1"

    def __init__(self):
        super(NotDefaultUVSet, self).__init__()

    def Name(self):
        return "NotDefaultUVSet"

    def Description(self):
        return "Mesh(s) is not setted default uv set(map1)"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def test(self, node):
        mesh = None
        try:
            mesh = OpenMaya.MFnMesh(node.object())
        except:
            return None

        if mesh.currentUVSetName() != NotDefaultUVSet.__DefaultSetName:
            return medic.PyReport(node)

        return None


def Create():
    return NotDefaultUVSet()
