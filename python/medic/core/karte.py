import os
from .. import collector
import copy
import fnmatch


class Karte(object):
    def __repr__(self):
        return "<Karte '%s'>" % self.name()

    def __init__(self, karte_data, filepath, testers):
        self.__name = None
        self.__filepath = None
        self.__description = ""
        self.__testers = {}
        self.__initKarte(karte_data, filepath, testers)

    def __initKarte(self, karte_data, filepath, testers):
        self.__name = karte_data.get("Name", "UNKNOWN")
        self.__description = karte_data.get("Description", "")
        self.__filepath = filepath
        tester_patterns = karte_data.get("Testers", [])

        for tester in testers:
            for pt in tester_patterns:
                if fnmatch.fnmatchcase(tester.name(), pt):
                    self.__testers[tester.name()] = tester
                    break

    def tester(self, tester_name):
        return self.__testers.get(tester_name, None)

    def testers(self):
        return copy.copy(self.__testers)

    def name(self):
        return self.__name

    def filepath(self):
        return self.__filepath

    def description(self):
        return self.__description

    def accept(self, visitor):
        visitor.visit(self)
