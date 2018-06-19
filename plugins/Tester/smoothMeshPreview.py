import medic
from maya import OpenMaya


class SmoothMeshPreview(medic.PyTester):
    def __init__(self):
        super(SmoothMeshPreview, self).__init__()

    def Name(self):
        return "SmoothMeshPreview"

    def Description(self):
        return "Mesh(s) is set 'smooth mesh preview' on"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def IsFixable(self):
        return True

    def testNode(self, node):
        plug = node.dg().findPlug("displaySmoothMesh")
        if not plug:
            return None

        if plug.asInt() == 0:
            return None

        return medic.PyReport(node)

    def fix(self, report, params):
        plug = report.node().dg().findPlug("displaySmoothMesh")
        plug.setInt(0)

        return True


def Create():
    return SmoothMeshPreview()
