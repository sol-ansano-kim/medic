import medic
from maya import OpenMaya
from maya import cmds

class IntermediateNode(medic.PyTester):
    def __init__(self):
        super(IntermediateNode, self).__init__()

    def Name(self):
        return "IntermediateNode"

    def Description(self):
        return "Intermediate node(s) exists"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kDagNode)

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


    def test(self, node):
        if not node.dag().isIntermediateObject():
            return None

        for i, f in enumerate(cmds.listHistory(node.name(), future=True)) or []:
            if i == 0:
                continue

            if "shape" in cmds.nodeType(f, inherited=True):
                return None

        return medic.PyReport(node)
       

def Create():
    return IntermediateNode()