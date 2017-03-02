from medic.core import testerBase
from maya import OpenMaya


class FaceAssigned(testerBase.TesterBase):
    Name = "FaceAssigned"

    def __init__(self):
        super(FaceAssigned, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh) or node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom)

    def Test(self, node):
        geom = None
        try:
            if node.object().hasFn(OpenMaya.MFn.kMesh):
                geom = OpenMaya.MFnMesh(node.object())
            elif node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom):
                geom = OpenMaya.MFnNurbsSurface(node.object())
        except:
            return (False, None)

        for i in range(node.dag().instanceCount(True)):
            objs = OpenMaya.MObjectArray()
            sid = OpenMaya.MIntArray()
            geom.getConnectedShaders(i, objs, sid)
            if objs.length() > 1:
                return (True, None)

        return (False, None)


Tester = FaceAssigned
