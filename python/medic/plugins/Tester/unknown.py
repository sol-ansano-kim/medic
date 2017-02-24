from medic.core import testerBase
from maya import OpenMaya


class UnknownNode(testerBase.TesterBase):
    Name = "UnknownNode"

    def __init__(self):
        super(UnknownNode, self).__init__()

    def Match(self, m_object):
        return True

    def Test(self, node):
        return node.dg().typeId().id() in [0x554e4b44, 0x554e4b4e, 0x554e4b54]


Tester = UnknownNode
