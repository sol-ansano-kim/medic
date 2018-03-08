import medic
from maya import OpenMaya
from maya import cmds
import sys


class FreezeVertex(medic.PyTester):
    def __init__(self):
        super(FreezeVertex, self).__init__()

    def Name(self):
        return "FreezeVertex"

    def Description(self):
        return "Not Frozen Verticies"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def test(self, node, geom = None):
        plug = node.dg().findPlug("pnts", True)
        tweaked = False
        for i in range(plug.numElements()):
            elm = plug.elementByPhysicalIndex(i)
            for j in range(3):
                if abs(elm.child(j).asDouble()) > sys.float_info.epsilon:
                    tweaked = True
                    break
            if tweaked:
                break
            
        if tweaked:
            return medic.PyReport(node)
        else:
            return None

    def IsFixable(self):
        return True

    def fix(self, report, params):
        obj_name = report.node().name()

        cmds.polyMoveVertex(obj_name, ch=1)

        return True

def Create():
    return FreezeVertex()
