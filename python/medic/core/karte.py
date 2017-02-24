import os
from .. import collector
import copy


class Karte(object):
    def __repr__(self):
        return "<Karte '%s'>" % self.name()

    def __init__(self, karte_data, filepath, testers):
        self.__name = None
        self.__filepath = None
        self.__description = ""
        self.__testers = []
        self.__initKarte(karte_data, filepath, testers)

    def __initKarte(self, karte_data, filepath, testers):
        self.__name = karte_data.get("Name", "UNKNOWN")
        self.__description = karte_data.get("Description", "")
        self.__filepath = filepath

        tester_list = karte_data.get("Testers", [])

        if not tester_list:
            return

        if isinstance(tester_list, basestring) and tester_list == "ALL":
            self.__testers = copy.copy(testers)
            return

        n_t_map = {}
        for t in testers:
            n_t_map[t.name()] = t

        if isinstance(tester_list, basestring):
            t = n_t_map.get(tester_list)
            if t:
                self.__testers.append(t)
                return

        if isinstance(tester_list, list):
            for tester_name in tester_list:
                t = n_t_map.get(tester_name)
                if t:
                    self.__testers.append(t)

    def testers(self):
        return copy.copy(self.__testers)

    def name(self):
        return self.__name

    def filepath(self):
        return self.__filepath

    def description(self):
        return self.__description
