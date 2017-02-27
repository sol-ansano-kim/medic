from medic.core import testerBase
from maya import OpenMaya


class NotFreezed(testerBase.TesterBase):
    Name = "NotFreezed"
    Identity = OpenMaya.MMatrix()

    def __init__(self):
        super(NotFreezed, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def Test(self, node):
        if node.dag().isInstanced():
            return (False, None)

        iden = OpenMaya.MMatrix()

        for p in node.parents():
            transform = p.dag().transformationMatrix()
            if not NotFreezed.Identity == transform:
                return (True, None)

        return (False, None)


Tester = NotFreezed
