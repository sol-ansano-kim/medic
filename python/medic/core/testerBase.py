PluginType = "Tester"


class TesterBase(object):
    Name = "TesterBase"
    Fixable = False

    @classmethod
    def IsFixable(klass):
        return klass.Fixable

    @classmethod
    def __repr__(klass):
        return "<Tester '%s'>" % klass.Name

    @classmethod
    def Create(klass):
        return klass()

    @classmethod
    def name(klass):
        return klass.Name

    def __init__(self):
        super(TesterBase, self).__init__()

    def Match(self, node):
        return False

    def Test(self, node):
        return (False, None)

    def GetParameters(self):
        return []

    def Fix(self, node, component, param_parser):
        return False


Tester = TesterBase
