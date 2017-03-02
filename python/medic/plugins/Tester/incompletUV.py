from medic.core import testerBase
from maya import OpenMaya


class IncompleteUV(testerBase.TesterBase):
    Name = "IncompleteUV"

    def __init__(self):
        super(IncompleteUV, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def Test(self, node):
        mesh = None
        try:
            mesh = OpenMaya.MFnMesh(node.object())
        except:
            return (False, None)

        uv_count = 0

        map_list = []
        mesh.getUVSetNames(map_list)

        for i, mp in enumerate(map_list):
            n_uvs = mesh.numUVs(mp)

            if i == 0:
                uv_count = n_uvs
            elif uv_count != n_uvs:
                return (True, None)

        if uv_count is 0:
            return (True, None)

        return (False, None)


Tester = IncompleteUV
