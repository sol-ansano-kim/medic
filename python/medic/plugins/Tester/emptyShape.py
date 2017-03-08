from medic.core import testerBase
from maya import OpenMaya


class EmptyShape(testerBase.TesterBase):
    Name = "EmptyShape"
    Fixable = True

    def __init__(self):
        super(EmptyShape, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom) or node.object().hasFn(OpenMaya.MFn.kNurbsCurve) or node.object().hasFn(OpenMaya.MFn.kMesh)

    def Test(self, node):
        nc = None
        if node.object().hasFn(OpenMaya.MFn.kMesh):
            om = node.dg().findPlug("outMesh", True)
            nc = om.asMDataHandle().asMesh()

        elif node.object().hasFn(OpenMaya.MFn.kNurbsCurve):
            cc = node.dg().findPlug("cc", True)
            nc = cc.asMDataHandle().asNurbsCurve()

        elif node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom):
            cc = node.dg().findPlug("cc", True)
            nc = cc.asMDataHandle().asNurbsSurface()

        if nc is not None and nc.isNull():
            return (True, None)

        return (False, None)

    def GetParameters(self):
        return []

    def Fix(self, node, component, parameterParser):
        if node.dg().isFromReferencedFile():
            return False

        if node.dg().isLocked():
            node.dg().setLocked(False)

        mod = OpenMaya.MDGModifier()
        mod.deleteNode(node.object())
        mod.doIt()
        return True


Tester = EmptyShape
