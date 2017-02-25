from medic.core import testerBase
from maya import OpenMaya


class OverFourSidedPolygon(testerBase.TesterBase):
    Name = "OverFourSidedPolygon"

    def __init__(self):
        super(OverFourSidedPolygon, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def Test(self, node):
        it = None
        try:
            it = OpenMaya.MItMeshPolygon(node.object())
        except:
            return False

        while (not it.isDone()):
            if it.polygonVertexCount() > 4:
                return True

            it.next()

        return False


Tester = OverFourSidedPolygon
