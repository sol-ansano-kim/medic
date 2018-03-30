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
        dg = node.dg()

        if not dg.hasAttribute("compInstObjGroups") or not dg.hasAttribute("compObjectGroups"):
            return None

        cio_plug = node.dg().findPlug("compInstObjGroups")
        cog_obj = node.dg().attribute("compObjectGroups")

        cio_plug.numConnectedElements()

        for i in range(cio_plug.numElements()):
            elm = cio_plug.elementByPhysicalIndex(i)
            cog_plug = elm.child(cog_obj)

            if not cog_plug.numConnectedElements():
                continue

            for j in range(cog_plug.numElements()):
                gelm = cog_plug.elementByPhysicalIndex(j)

                arr = OpenMaya.MPlugArray()
                if not gelm.connectedTo(arr, False, True):
                    continue

                for n in range(arr.length()):
                    if arr[n].node().hasFn(OpenMaya.MFn.kShadingEngine):
                        return medic.PyReport(node)

        return None


def Create():
    return FaceAssigned()
