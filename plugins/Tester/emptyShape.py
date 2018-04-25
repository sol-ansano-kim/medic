import medic
from maya import OpenMaya
from maya import cmds


class EmptyShape(medic.PyTester):
    def __init__(self):
        super(EmptyShape, self).__init__()

    def Name(self):
        return "EmptyShape"

    def Description(self):
        return "Empty shape(s)"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom) or node.object().hasFn(OpenMaya.MFn.kNurbsCurve) or node.object().hasFn(OpenMaya.MFn.kMesh)

    def test(self, node):
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
            return medic.PyReport(node)

        return None

    def IsFixable(self):
        return True

    def fix(self, report, params):
        node = report.node()
        if node.dg().isFromReferencedFile():
            return False

        if node.dg().isLocked():
            node.dg().setLocked(False)

        cmds.delete(node.name())

        return True


def Create():
    return EmptyShape()
