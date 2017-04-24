from medic.core import testerBase
from maya import OpenMaya


class HasInputGeometry(testerBase.TesterBase):
    Name = "HasInputGeometry"
    Description = "Shape(s) has an input connection"

    def __init__(self):
        super(HasInputGeometry, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom) or node.object().hasFn(OpenMaya.MFn.kNurbsCurve) or node.object().hasFn(OpenMaya.MFn.kMesh)

    def _plugHasSourceConnection(self, plug):
        arr = OpenMaya.MPlugArray()
        plug.connectedTo(arr, True, False)

        return arr.length() > 0

    def Test(self, node):
        plug_name = None
        if node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom) or node.object().hasFn(OpenMaya.MFn.kNurbsCurve):
            plug_name = "create"

        elif node.object().hasFn(OpenMaya.MFn.kMesh):
            plug_name = "inMesh"
            
        if not plug_name:
            return (False, None)

        return (self._plugHasSourceConnection(node.dg().findPlug(plug_name)), None)


Tester = HasInputGeometry


