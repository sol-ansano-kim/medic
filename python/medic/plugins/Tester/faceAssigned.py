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

        if obj_grp.numConnectedElements() > 0:
            return True

        return False


Tester = FaceAssigned
