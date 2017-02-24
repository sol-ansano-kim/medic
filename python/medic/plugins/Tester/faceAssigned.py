from medic.core import testerBase
from maya import OpenMaya


class FaceAssigned(testerBase.TesterBase):
    Name = "FaceAssigned"

    def __init__(self):
        super(FaceAssigned, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kDagNode)

    def Test(self, node):
        inst_grp = node.dg().findPlug("instObjGroups", True)
        if not inst_grp:
            return False

        obj_grp = None

        for i in range(inst_grp.numChildren()):
            child = inst_grp.child(i)
            if child.partialName() == "iog[-1].og":
                obj_grp = child
                break

        if not obj_grp:
            return False

        connected_count = obj_grp.numConnectedElements()
        if connected_count is 0:
            return False

        for i in range(connected_count):
            has_connection_plug = obj_grp.connectionByPhysicalIndex(i)
            arr = OpenMaya.MPlugArray()

            if not has_connection_plug.connectedTo(arr, False, True):
                continue

            for j in range(arr.length()):
                if arr[j].node().hasFn(OpenMaya.MFn.kShadingEngine):
                    return True

        return False


Tester = FaceAssigned
