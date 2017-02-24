from medic.core import testerBase


class Referenced(testerBase.TesterBase):
    Name = "Referenced"

    def __init__(self):
        super(Referenced, self).__init__()

    def Match(self, node):
        return True

    def Test(self, node):
        return node.dg().isFromReferencedFile()


Tester = Referenced
