from medic.core import testerBase
import re


RE_NS = re.compile("[:]")


class HasNamespace(testerBase.TesterBase):
    Name = "HasNamespace"

    def __init__(self):
        super(HasNamespace, self).__init__()

    def Match(self, m_object):
        return True

    def Test(self, node):
        return True if (not node.dg().isFromReferencedFile()) and (RE_NS.search(node.name())) else False


Tester = HasNamespace
