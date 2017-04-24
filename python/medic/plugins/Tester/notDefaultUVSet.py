from medic.core import testerBase
from maya import OpenMaya


class NotDefaultUVSet(testerBase.TesterBase):
    Name = "NotDefaultUVSet"
    Description = "Mesh(s) is not setted default uv set(map1)"
    __DefaultSetName = "map1"

    def __init__(self):
        super(NotDefaultUVSet, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def Test(self, node):
        mesh = None
        try:
            mesh = OpenMaya.MFnMesh(node.object())
        except:
            return (False, None)

        return (mesh.currentUVSetName() != NotDefaultUVSet.__DefaultSetName, None)


Tester = NotDefaultUVSet
