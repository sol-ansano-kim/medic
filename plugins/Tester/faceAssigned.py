import medic
from maya import OpenMaya


class FaceAssigned(medic.PyTester):
    def __init__(self):
        super(FaceAssigned, self).__init__()

    def Name(self):
        return "FaceAssigned"

    def Description(self):
        return "Face assigned mesh(s)"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh) or node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom)

    def test(self, node):
        geom = None
        try:
            if node.object().hasFn(OpenMaya.MFn.kMesh):
                geom = OpenMaya.MFnMesh(node.object())
            elif node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom):
                geom = OpenMaya.MFnNurbsSurface(node.object())
        except:
            return None

        for i in range(node.dag().instanceCount(True)):
            objs = OpenMaya.MObjectArray()
            sid = OpenMaya.MIntArray()
            geom.getConnectedShaders(i, objs, sid)

            if objs.length() > 1:
                return medic.PyReport(node)

        return None


def Create():
    return FaceAssigned()
