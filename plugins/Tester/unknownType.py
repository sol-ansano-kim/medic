import medic
from maya import OpenMaya
from maya import cmds


class UnknownType(medic.PyTester):
    def __init__(self):
        super(UnknownType, self).__init__()

    def Name(self):
        return "UnknownType"

    def Description(self):
        return "Unknown type node(s) exists"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kUnknown) or node.object().hasFn(OpenMaya.MFn.kUnknownTransform) or node.object().hasFn(OpenMaya.MFn.kUnknownDag)

    def IsFixable(self):
        return True

    def test(self, node):
        return medic.PyReport(node)

    def fix(self, report, params):
        node = report.node()
        if node.dg().isFromReferencedFile():
            return False

        if node.dg().isLocked():
            node.dg().setLocked(False)

        cmds.delete(node.name())

        return True


def Create():
    return UnknownType()
