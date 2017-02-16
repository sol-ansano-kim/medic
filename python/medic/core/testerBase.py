PluginType = "Tester"


class TesterBase(object):
    @classmethod
    def Create(klass):
        return klass()

    def __init__(self):
        super(TesterBase, self).__init__()

    def Match(self, m_object):
        return False

    def Test(self, node):
        return False


Class = TesterBase
