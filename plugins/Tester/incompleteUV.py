import medic
from maya import OpenMaya


class IncompleteUV(medic.PyTester):
    def __init__(self):
        super(IncompleteUV, self).__init__()

    def Name(self):
        return "IncompleteUV"

    def Description(self):
        return "Mesh(s) has incomplete UV(s)"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def test(self, node):
        mesh = None
        try:
            mesh = OpenMaya.MFnMesh(node.object())
        except:
            return None

        uv_count = 0

        map_list = []
        mesh.getUVSetNames(map_list)

        for i, mp in enumerate(map_list):
            n_uvs = mesh.numUVs(mp)

            if i == 0:
                uv_count = n_uvs
            elif uv_count != n_uvs:
                return medic.PyReport(node)

        if uv_count is 0:
            return medic.PyReport(node)

        return None


def Create():
    return IncompleteUV()
