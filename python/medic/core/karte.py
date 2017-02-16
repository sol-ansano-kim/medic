import os
from .. import collector
import copy


class Karte(object):
    def __init__(self):
        self.Name = None
        self.__filepath = None
        self.__description = ""
        self.__testers = []

    def addTester(self, tester):
        self.__testers.append(tester)

    def removeTester(self, tester):
        if tester in self.__testers:
            self.__testers.remove(tester)

    def testers(self):
        return copy.copy(self.__testers)

    def description(self):
        return self.__description
