from medic.core import testerBaset


class UnknownType(testerBase.TesterBase):
    Name = "UnknownType"

    def __init__(self):
        super(UnknownType, self).__init__()

    def Match(self, m_object):
        return True

    def Test(self, node):
        return node.dg().typeId().id() in [0x554e4b44, 0x554e4b4e, 0x554e4b54]


Tester = UnknownType
