import medic
from maya import OpenMaya


class ConstructionHistory(medic.PyTester):
    def __init__(self):
        super(ConstructionHistory, self).__init__()

    def Name(self):
        return "ConstructionHistory"

    def Description(self):
        return "Shape has construction history"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom) or node.object().hasFn(OpenMaya.MFn.kNurbsCurve) or node.object().hasFn(OpenMaya.MFn.kMesh)

    def test(self, node):
        plug_name = None
        if node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom) or node.object().hasFn(OpenMaya.MFn.kNurbsCurve):
            plug_name = "create"

        elif node.object().hasFn(OpenMaya.MFn.kMesh):
            plug_name = "inMesh"

        if not plug_name:
            return None

        if self.__plugHasSourceConnection(node.dg().findPlug(plug_name)):
            return medic.PyReport(node)

        return None

    def __plugHasSourceConnection(self, plug):
        if not plug:
            return False

        arr = OpenMaya.MPlugArray()
        plug.connectedTo(arr, True, False)

        return arr.length() > 0


def Create():
    return ConstructionHistory()
