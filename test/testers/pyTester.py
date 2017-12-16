import _medic

class UnknownType(_medic.Tester):
    def __init__(self):
        super(UnknownType, self).__init__()

    def Name(self):
        return "TesterPY"

    def Description(self):
        return "python tester"

    def Match(self, node):
        print(">> Call Match")
        print(">> Node is %s" % (node))
        return True

    def IsFixable(self):
        return True

    def test(self, node):
        print(">> Call test")
        print(">> Node is %s" % (node))
        new = Report(node)
        return new

    def fix(self, report, params):
        print(">> Call fix")
        print(report)
        print(params)
        return True

    def GetParameters(self):
        return ParamContainer()


def Create():
    return UnknownType()
