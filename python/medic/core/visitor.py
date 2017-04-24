import copy


class Visitor(object):
    def __init__(self):
        super(Visitor, self).__init__
        self.__result = {}

    def addResult(self, tester, node):
        if not self.__result.has_key(tester.name()):
            self.__result[tester.name()] = set()
        self.__result[tester.name()].add(node)

    def resetResults(self):
        self.__result = {}

    def result(self, tester_name):
        return self.__result.get(tester_name, set())

    def results(self):
        return copy.copy(self.__result)

    def visit(self, karte):
        pass