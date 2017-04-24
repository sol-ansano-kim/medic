from medic.core import testerBase


class Referenced(testerBase.TesterBase):
    Name = "Referenced"
    Description = "Refrenced node(s) exists"

    def __init__(self):
        super(Referenced, self).__init__()

    def Match(self, node):
        return True

    def Test(self, node):
        return (node.dg().isFromReferencedFile(), None)


Tester = Referenced
