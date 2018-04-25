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

    @staticmethod
    def __TestObjGrp(node, parentPlug, childPlug):
        dg = node.dg()

        if not dg.hasAttribute(parentPlug) or not dg.hasAttribute(childPlug):
            return False

        io_plug = node.dg().findPlug(parentPlug)
        og_obj = node.dg().attribute(childPlug)

        for i in range(io_plug.numElements()):
            elm = io_plug.elementByPhysicalIndex(i)
            og_plug = elm.child(og_obj)

            if not og_plug.numConnectedElements():
                continue

            for j in range(og_plug.numElements()):
                gelm = og_plug.elementByPhysicalIndex(j)

                arr = OpenMaya.MPlugArray()
                if not gelm.connectedTo(arr, False, True):
                    continue

                for n in range(arr.length()):
                    if arr[n].node().hasFn(OpenMaya.MFn.kShadingEngine):
                        return True

        return False

    def test(self, node):
        if FaceAssigned.__TestObjGrp(node, "compInstObjGroups", "compObjectGroups"):
            return medic.PyReport(node)

        if FaceAssigned.__TestObjGrp(node, "instObjGroups", "objectGroups"):
            return medic.PyReport(node)

        return None

def Create():
    return FaceAssigned()
