from medic.core import testerBase
from maya import OpenMaya


class SmoothMeshPreview(testerBase.TesterBase):
    Name = "SmoothMeshPreview"
    Description = "Mesh(s) is set 'smooth mesh preview' on"
    Fixable = True

    def __init__(self):
        super(SmoothMeshPreview, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def Test(self, node):
        plug = node.dg().findPlug("displaySmoothMesh")
        if not plug:
            return (False, None)

        return (plug.asInt() != 0, None)

    def Fix(self, node, component, parameterParser):
        plug = node.dg().findPlug("displaySmoothMesh")
        plug.setInt(0)

        return True


Tester = SmoothMeshPreview
