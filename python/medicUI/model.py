import medic
from Qt import QtCore


DisplayRole = QtCore.Qt.DisplayRole
KarteRole = QtCore.Qt.UserRole + 1
KarteItemRole = QtCore.Qt.UserRole + 2
KarteTesterItemsRole = QtCore.Qt.UserRole + 3
VisitorRole = QtCore.Qt.UserRole + 4
TesterRole = QtCore.Qt.UserRole + 5
TesterItemRole = QtCore.Qt.UserRole + 6
StatusRole = QtCore.Qt.UserRole + 7
ReportRole = QtCore.Qt.UserRole + 8
ReportItemRole = QtCore.Qt.UserRole + 9
NodeRole = QtCore.Qt.UserRole + 10


Ready = 0
Success = 1
Failure = 2


class ReportItem(object):
    def __init__(self, report):
        self.__report = report

    def report(self):
        return self.__report

    def node(self):
        return self.__report.node()

    def name(self):
        return self.__report.node().name()


class TesterItem(object):
    def __init__(self, tester):
        self.__tester = tester
        self.__status = Ready
        self.__reports = []

    def reports(self):
        return self.__reports

    def reset(self):
        self.__status = Ready
        self.__reports = []

    def status(self):
        return self.__status

    def tester(self):
        return self.__tester

    def name(self):
        return self.__tester.Name()

    def description(self):
        return self.__tester.Description()

    def parameters(self):
        return self.__tester.GetParameters()

    def isFixable(self):
        return self.__tester.IsFixable()

    def fix(self, report, param):
        if report not in self.__reports:
            return False

        return self.__tester.fix(report.report(), param)

    def removeReports(self, reports):
        for report in reports:
            if report in self.__reports:
                self.__reports.remove(report)

        if not self.__reports:
            self.__status = Success

    def test(self, karte, visitor):
        self.__status = Ready

        visitor.test(karte, self.__tester)
        self.__reports = map(ReportItem, visitor.report(self.__tester))

        if not self.__reports:
            self.__status = Success
        else:
            self.__status = Failure


class KarteItem(object):
    def __init__(self, karte, testerItems):
        self.__karte = karte
        self.__visitor = medic.Visitor()
        self.__tester_items = testerItems

    def testerItems(self):
        return self.__tester_items

    def testAll(self, testerCallback=None):
        self.reset()
        for tester in self.__tester_items:
            tester.test(self.__karte, self.__visitor)
            if testerCallback:
                testerCallback()

    def reset(self):
        self.__visitor.reset()
        for tester in self.__tester_items:
            tester.reset()

    def status(self):
        has_ready = False
        for tester in self.__tester_items:
            if tester.status() is Failure:
                return Failure
            if tester.status() is Ready:
                has_ready = True
        return Success if not has_ready else Ready

    def visitor(self):
        return self.__visitor

    def karte(self):
        return self.__karte

    def name(self):
        return self.__karte.Name()


class KarteModel(QtCore.QAbstractListModel):
    def __init__(self, parent=None):
        super(KarteModel, self).__init__(parent=parent)
        self.__manager = None
        self.__karte_items = []
        self.__initialize()

    def __initialize(self):
        self.beginResetModel()
        self.__manager = medic.PluginManager()

        all_testers = map(lambda x: self.__manager.tester(x), self.__manager.testerNames())
        for karte_name in self.__manager.karteNames():
            karte = self.__manager.karte(karte_name)
            tester_items = []
            for tester in all_testers:
                if karte.hasTester(tester):
                    tester_items.append(TesterItem(tester))

            self.__karte_items.append(KarteItem(karte, tester_items))

        self.endResetModel()

    def rowCount(self, parent=None):
        return len(self.__karte_items)

    def data(self, index, role):
        if index.row() < 0 or index.row() > self.rowCount():
            return None

        if role == DisplayRole:
            return self.__karte_items[index.row()].name()

        if role == KarteTesterItemsRole:
            return self.__karte_items[index.row()].testerItems()

        if role == KarteItemRole:
            return self.__karte_items[index.row()]

        if role == VisitorRole:
            return self.__karte_items[index.row()].visitor()

        if role == KarteRole:
            return self.__karte_items[index.row()].karte()


class TesterModel(QtCore.QAbstractListModel):
    def __init__(self, parent=None):
        super(TesterModel, self).__init__(parent=parent)
        self.__tester_items = []

    def setTesterItems(self, testerItems):
        self.beginResetModel()
        self.__tester_items = testerItems
        self.endResetModel()

    def rowCount(self, parent=None):
        return len(self.__tester_items)

    def data(self, index, role):
        if index.row() < 0 or index.row() > self.rowCount():
            return None

        if role == DisplayRole:
            return self.__tester_items[index.row()].name()

        if role == TesterRole:
            return self.__tester_items[index.row()].tester()

        if role == TesterItemRole:
            return self.__tester_items[index.row()]

        if role == StatusRole:
            return self.__tester_items[index.row()].status()


class ReportModel(QtCore.QAbstractListModel):
    def __init__(self, parent=None):
        super(ReportModel, self).__init__(parent=parent)
        self.__report_items = []

    def setReportItems(self, report_items):
        self.beginResetModel()
        self.__report_items = report_items
        self.endResetModel()

    def rowCount(self, parent=None):
        return len(self.__report_items)

    def data(self, index, role):
        if index.row() < 0 or index.row() > self.rowCount():
            return None

        if role == DisplayRole:
            return self.__report_items[index.row()].name()

        if role == ReportRole:
            return self.__report_items[index.row()].report()

        if role == ReportItemRole:
            return self.__report_items[index.row()]

        if role == NodeRole:
            return self.__report_items[index.row()].node()
