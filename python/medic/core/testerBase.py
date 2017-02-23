PluginType = "Tester"


class TesterBase(object):
    Name = "TesterBase"

    @classmethod
    def Create(klass):
        return klass()

    @classmethod
    def name(klass):
        return klass.Name

    def __init__(self):
        super(TesterBase, self).__init__()

    def Match(self, m_object):
        return False

    def Test(self, node):
        return False


Tester = TesterBase
