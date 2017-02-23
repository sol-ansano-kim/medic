from medic.core import testerBase
from maya import OpenMaya


class UniqueName(testerBase.TesterBase):
    Name = "UniqueName"

    def __init__(self):
        super(UniqueName, self).__init__()

    def Match(self, m_object):
        return m_object.hasFn(OpenMaya.MFn.kDagNode)

    def Test(self, m_object):
        return self.dg().hasUniqueName()


Tester = UniqueName
