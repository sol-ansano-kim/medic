from Qt import QtWidgets, QtCore, QtGui
from . import model
from . import delegate
from . import functions
import medic
import os
import re


IconDir = os.path.abspath(os.path.join(__file__, "../icons"))


class ParameterFunctions():
    @staticmethod
    def SetParmeterValue(param_container, pram_dict):
        for prm in pram_dict:
            if prm["function"]:
                prm["function"](param_container, prm["name"], prm["widget"])

    @staticmethod
    def SetInt(param, name, widget):
        t = widget.text()
        if not t:
            t = 0
        param.set(name, int(t))

    @staticmethod
    def SetFloat(param, name, widget):
        t = widget.text()
        if not t:
            t = 0
        param.set(name, float(t))

    @staticmethod
    def SetBool(param, name, widget):
        param.set(name, widget.isChecked())

    @staticmethod
    def SetString(param, name, widget):
        param.set(name, str(widget.text()))

    @staticmethod
    def CreateWidget(info):
        name, label, parm_type, default = info

        if parm_type is medic.Types.Null or\
           parm_type is medic.Types.BoolArray or\
           parm_type is medic.Types.IntArray or\
           parm_type is medic.Types.FloatArray or\
           parm_type is medic.Types.StringArray:
            print "This type parameter is not supported yet : %s" % parm_type
            return None, None

        widget = None
        function = None

        if parm_type == medic.Types.Bool:
            widget = QtWidgets.QCheckBox()
            widget.setChecked(default)
            function = ParameterFunctions.SetBool

        elif parm_type == medic.Types.Int:
            widget = NumericLine.CreateIntLine()
            widget.setText(str(default))
            function = ParameterFunctions.SetInt

        elif parm_type == medic.Types.Float:
            widget = NumericLine.CreateFloatLine()
            widget.setText(str(default))
            function = ParameterFunctions.SetFloat

        elif parm_type == medic.Types.String:
            widget = QtWidgets.QLineEdit()
            widget.setText(default)
            function = ParameterFunctions.SetString

        widget.setObjectName("parameter_widget")

        return widget, function


class BrowserButtonWidget(QtWidgets.QFrame):
    BackClicked = QtCore.Signal()
    NextClicked = QtCore.Signal()

    def __init__(self, parent=None):
        super(BrowserButtonWidget, self).__init__(parent=parent)
        self.setObjectName("medic_browser_buttons_widget")
        self.__back_button = None
        self.__next_button = None

        self.__makeWidgets()

    def __makeWidgets(self):
        main_layout = QtWidgets.QHBoxLayout()
        self.setLayout(main_layout)

        self.__back_button = QtWidgets.QPushButton()
        self.__next_button = QtWidgets.QPushButton()
        self.__back_button.setObjectName("medic_browser_back")
        self.__next_button.setObjectName("medic_browser_next")

        main_layout.addWidget(self.__back_button)
        main_layout.addWidget(self.__next_button)
        main_layout.setSpacing(1)

        self.__back_button.clicked.connect(self.BackClicked.emit)
        self.__next_button.clicked.connect(self.NextClicked.emit)

    def setBackEnabled(self, v):
        self.__back_button.setEnabled(v)

    def setNextEnabled(self, v):
        self.__next_button.setEnabled(v)


class CurrentKarteLabel(QtWidgets.QLabel):
    def __init__(self, parent=None):
        super(CurrentKarteLabel, self).__init__(parent=parent)
        self.setObjectName("medic_current_karte")


class StatusLabel(QtWidgets.QLabel):
    def __init__(self, parent=None):
        super(StatusLabel, self).__init__(parent=parent)
        self.setObjectName("status_label")
        self.setFixedWidth(70)
        self.__ready_icon = QtGui.QPixmap(os.path.join(IconDir, "success.png")).scaled(16, 16)
        self.__success_icon = QtGui.QPixmap(os.path.join(IconDir, "success.png")).scaled(16, 16)
        self.__failure_icon = QtGui.QPixmap(os.path.join(IconDir, "failure.png")).scaled(16, 16)
        self.setStatus(model.Ready)

    def setStatus(self, status):
        if status is model.Ready:
            self.setText("<font color='#b0b0b0'>Ready</font>")
        elif status is model.Success:
            self.setText("<font color='#1cc033'>Success</font>")
        else:
            self.setText("<font color='#eb2b66'>Failure</font>")


class TesterList(QtWidgets.QListView):
    TesterChanged = QtCore.Signal("QModelIndex")
    SingleTestTriggered = QtCore.Signal()

    def __init__(self, parent=None):
        super(TesterList, self).__init__(parent=parent)
        self.setObjectName("medic_tester_list")
        self.setAlternatingRowColors(True)
        self.setUniformItemSizes(True)
        self.source_model = model.TesterModel()
        self.delegate = delegate.TesterDelegate()
        self.setItemDelegate(self.delegate)
        self.setModel(self.source_model)
        self.__current_tester = None

    def currentTester(self):
        return self.__current_tester

    def reset(self):
        self.clearSelection()
        self.__current_tester = None

    def selectionChanged(self, selected, deselected):
        indexes = selected.indexes()
        if not indexes:
            self.clearSelection()
            self.__current_tester = None
            self.TesterChanged.emit(None)
        else:
            self.__current_tester = self.source_model.data(indexes[0], model.TesterItemRole)
            self.TesterChanged.emit(indexes[0])

        super(TesterList, self).selectionChanged(selected, deselected)

    def mousePressEvent(self, evnt):
        super(TesterList, self).mousePressEvent(evnt)

        if QtCore.Qt.MouseButton.LeftButton == evnt.button():
            index = self.indexAt(evnt.pos())
            if index.row() < 0:
                self.__current_tester = None
                self.clearSelection()

        elif QtCore.Qt.MouseButton.RightButton == evnt.button():
            menu = QtWidgets.QMenu(self)
            test = QtWidgets.QAction("Single Test", menu)
            menu.addAction(test)
            pos = self.mapToGlobal(evnt.pos())
            menu.popup(QtCore.QPoint(pos.x() - 10, pos.y() - 10))
            test.triggered.connect(self.__testTriggered)

    def __testTriggered(self):
        self.SingleTestTriggered.emit()


class KarteList(QtWidgets.QListView):
    KarteChanged = QtCore.Signal("QModelIndex")

    def __init__(self, parent=None):
        super(KarteList, self).__init__(parent=parent)
        self.setObjectName("medic_karte_list")
        self.setAlternatingRowColors(True)
        self.setUniformItemSizes(True)
        self.source_model = model.KarteModel()
        self.delegate = delegate.KarteDelegate()
        self.setModel(self.source_model)
        self.setItemDelegate(self.delegate)
        self.__current_karte = None

    def currentKarte(self):
        return self.__current_karte

    def selectionChanged(self, selected, deselected):
        indexes = selected.indexes()
        if not indexes:
            self.clearSelection()
            self.__current_karte = None
            self.KarteChanged.emit(None)
        else:
            self.__current_karte = self.source_model.data(indexes[0], model.KarteItemRole)
            self.KarteChanged.emit(indexes[0])

        super(KarteList, self).selectionChanged(selected, deselected)

    def mousePressEvent(self, evnt):
        if QtCore.Qt.MouseButton.LeftButton == evnt.button():
            index = self.indexAt(evnt.pos())
            if index.row() < 0:
                self.clearSelection()
                self.__current_karte = None

        super(KarteList, self).mousePressEvent(evnt)


class NumericLine(QtWidgets.QLineEdit):
    RegexInt = re.compile("[^0-9-]")
    RegexFloat = re.compile("[^0-9-.]")

    def __init__(self, parent=None):
        super(NumericLine, self).__init__(parent)
        self.__regex = None
        self.textEdited.connect(self.__regexCheck)

    def __regexCheck(self, txt):
        if self.__regex and txt:
            self.setText(self.__regex.sub("", txt))

    @staticmethod
    def CreateIntLine():
        e = NumericLine()
        e.__regex = NumericLine.RegexInt
        return e

    @staticmethod
    def CreateFloatLine():
        e = NumericLine()
        e.__regex = NumericLine.RegexFloat
        return e


class ReportList(QtWidgets.QListView):
    def __init__(self, parent=None):
        super(ReportList, self).__init__(parent)
        self.source_model = model.ReportModel()
        self.setModel(self.source_model)
        self.setSelectionMode(QtWidgets.QAbstractItemView.ExtendedSelection)
        self.delegate = delegate.ReportDelegate()
        self.setItemDelegate(self.delegate)
        self.setAlternatingRowColors(True)

    def setReportItems(self, report_items):
        self.source_model.setReportItems(report_items)

    def selectionChanged(self, selected, deselected):
        indexes = selected.indexes()
        functions.ClearSelection()

        for index in self.selectedIndexes():
            report = self.source_model.data(index, model.ReportRole)
            report.addSelection()

        super(ReportList, self).selectionChanged(selected, deselected)

    def mousePressEvent(self, evnt):
        if QtCore.Qt.MouseButton.LeftButton == evnt.button():
            index = self.indexAt(evnt.pos())
            if index.row() < 0:
                self.clearSelection()

        super(ReportList, self).mousePressEvent(evnt)


class TesterDetailWidget(QtWidgets.QWidget):
    ReportsChanged = QtCore.Signal()

    def __init__(self, parent=None):
        super(TesterDetailWidget, self).__init__(parent)
        self.setObjectName("tester_detail_widget")
        self.__tester_item = None
        self.__params = []
        self.__param_container = None

        self.__qt_top_layout = None
        self.__qt_parameter_layout = None
        self.__qt_bottom_layout = None
        self.__qt_test_label = None
        self.__qt_report_list = None
        self.__qt_fix_selected_button = None
        self.__qt_fix_all_button = None

        self.__createWidgets()
        self.__clear()

    def onReset(self):
        self.__clear()

    def reset(self):
        self.__clear()

    def setTesterItem(self, testerItem):
        self.__tester_item = testerItem
        self.__setTester(self.__tester_item)
        self.__setReportItems(self.__tester_item.reports())

    def __setTester(self, testerItem):
        self.__setTesterName(testerItem.name())
        self.__setDescription(testerItem.description())
        self.__clearParameters()
        self.__setParameters(testerItem.parameters())
        self.__setFixable(testerItem.isFixable())

    def __setReportItems(self, report_items):
        self.__qt_report_list.setReportItems(report_items)
        if not report_items:
            self.__setFixable(False)

    def __createWidgets(self):
        main_layout = QtWidgets.QVBoxLayout()
        self.setLayout(main_layout)
        main_layout.setContentsMargins(0, 0, 0, 0)

        # frame
        frame = QtWidgets.QFrame()
        frame.setObjectName("detail_frame")
        main_layout.addWidget(frame)
        frame_layout = QtWidgets.QVBoxLayout()
        frame.setLayout(frame_layout)
        frame_layout.setContentsMargins(10, 10, 10, 10)

        # layout
        self.__qt_parameter_layout = QtWidgets.QVBoxLayout()
        button_layout = QtWidgets.QHBoxLayout()

        # widgets
        self.__qt_tester_label = QtWidgets.QLabel()
        self.__qt_description = QtWidgets.QTextEdit()
        self.__qt_description.setFixedHeight(50)
        self.__qt_description.setReadOnly(True)
        self.__qt_tester_label.setObjectName("detail_tester_label")
        self.__qt_description.setObjectName("detail_tester_description")
        self.__qt_report_list = ReportList()
        self.__qt_report_list.setObjectName("detial_report_list")
        self.__qt_fix_selected_button = QtWidgets.QPushButton("Fix Selected")
        self.__qt_fix_all_button = QtWidgets.QPushButton("Fix All")
        self.__qt_fix_selected_button.setObjectName("detail_button")
        self.__qt_fix_all_button.setObjectName("detail_button")
        self.__qt_fix_selected_button.setMaximumWidth(100)
        self.__qt_fix_all_button.setMaximumWidth(100)

        button_layout.addWidget(self.__qt_fix_selected_button)
        button_layout.addWidget(self.__qt_fix_all_button)

        frame_layout.addWidget(self.__qt_tester_label)
        frame_layout.addSpacing(20)
        frame_layout.addWidget(self.__qt_description)
        frame_layout.addWidget(self.__qt_report_list)
        frame_layout.addLayout(self.__qt_parameter_layout)
        frame_layout.addLayout(button_layout)

        self.__qt_fix_all_button.clicked.connect(self.__fixAll)
        self.__qt_fix_selected_button.clicked.connect(self.__fixSelected)

    def __clear(self):
        self.__tester_item = None
        self.__qt_report_list.setReportItems([])
        self.__setTesterName("")
        self.__setFixable(False)
        self.__setDescription("")
        self.__clearParameters()

    def __setTesterName(self, name):
        self.__qt_tester_label.setText(name)

    def __setDescription(self, desc):
        self.__qt_description.setText(desc)

    def __setFixable(self, enable):
        self.__qt_fix_selected_button.setEnabled(enable)
        self.__qt_fix_all_button.setEnabled(enable)

    def __clearLayout(self, layout):
        while (True):
            item = layout.takeAt(0)
            if item:
                l = item.layout()
                w = item.widget()
                if l:
                    self.__clearLayout(l)
                if w:
                    layout.removeWidget(w)
                    w.setParent(None)

            else:
                break

    def __clearParameters(self):
        self.__params = []
        self.__param_container = None
        self.__clearLayout(self.__qt_parameter_layout)

    def __setParameters(self, params):
        self.__param_container = params
        for info in params.getParamInfos():
            p_name, p_label, p_type, p_default = info
            widget, function = ParameterFunctions.CreateWidget(info)
            if widget:
                layout = QtWidgets.QHBoxLayout()
                label = QtWidgets.QLabel(p_label)
                label.setObjectName("parameter_label")
                layout.addWidget(label)
                layout.addWidget(widget)

                self.__params.append({"name": p_name, "widget": widget, "function": function})
                self.__qt_parameter_layout.addLayout(layout)

    def __fixAll(self):
        if self.__tester_item:
            ParameterFunctions.SetParmeterValue(self.__param_container, self.__params)

            remove_items = []

            for report in self.__tester_item.reports():
                if self.__tester_item.fix(report, self.__param_container):
                    remove_items.append(report)

            self.__tester_item.removeReports(remove_items)

            self.__setReportItems(self.__tester_item.reports())

            self.ReportsChanged.emit()

    def __fixSelected(self):
        if self.__tester_item:
            ParameterFunctions.SetParmeterValue(self.__param_container, self.__params)

            remove_items = []
            all_reports = self.__tester_item.reports()

            for index in map(lambda x: x.row(), self.__qt_report_list.selectedIndexes()):
                report = all_reports[index]
                if self.__tester_item.fix(report, self.__param_container):
                    remove_items.append(report)

            self.__tester_item.removeReports(remove_items)

            self.__setReportItems(self.__tester_item.reports())

            self.ReportsChanged.emit()


class TopBarWidget(QtWidgets.QFrame):
    BackClicked = QtCore.Signal()
    NextClicked = QtCore.Signal()

    def __init__(self, parent=None):
        super(TopBarWidget, self).__init__(parent=parent)
        self.setObjectName("medic_top_bar")
        self.__browser_button_widget = None
        self.__current_karte_label = None
        self.reset_button = None
        self.test_button = None
        self.status_label = None
        self.__phase_items = {}
        self.__phase = 0

        self.__makeWidgets()
        self.setPhase(0)

    def setBrowserButtonEnabled(self, prevValue, nextValue):
        self.__browser_button_widget.setBackEnabled(prevValue)
        self.__browser_button_widget.setNextEnabled(nextValue)

    def setCurrentKarteName(self, name):
        self.__current_karte_label.setText(name)

    def phase(self):
        return self.__phase

    def next(self):
        self.setPhase(self.__phase + 1)

    def back(self):
        self.setPhase(self.__phase - 1)

    def setPhase(self, phase):
        self.__phase = phase
        for p, items in self.__phase_items.iteritems():
            if p == self.__phase:
                for item in items:
                    item.show()
            else:
                for item in items:
                    item.hide()
        
    def __makeWidgets(self):
        main_layout = QtWidgets.QVBoxLayout()
        main_layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(main_layout)

        horizon_layout = QtWidgets.QHBoxLayout()
        horizon_layout.setSpacing(10)
        horizon_layout.setContentsMargins(0, 0, 0, 0)
        self.__browser_button_widget = BrowserButtonWidget()
        self.reset_button = QtWidgets.QPushButton()
        self.test_button = QtWidgets.QPushButton()
        self.reset_button.setObjectName("reset_button")
        self.test_button.setObjectName("test_button")
        self.status_label = StatusLabel()
        self.__current_karte_label = CurrentKarteLabel()

        self.__phase_items[1] = [self.reset_button, self.test_button, self.status_label]
        
        horizon_layout.addWidget(self.__browser_button_widget)
        horizon_layout.addSpacing(20)
        horizon_layout.addWidget(self.reset_button)
        horizon_layout.addWidget(self.test_button)
        horizon_layout.addWidget(self.status_label)
        horizon_layout.addStretch(9999)
        horizon_layout.addWidget(self.__current_karte_label)

        main_layout.addLayout(horizon_layout)

        self.__browser_button_widget.BackClicked.connect(self.BackClicked.emit)
        self.__browser_button_widget.NextClicked.connect(self.NextClicked.emit)


class MainWidget(QtWidgets.QWidget):
    ConditionChanged = QtCore.Signal(bool, bool)
    KarteChanged = QtCore.Signal(str)
    StatusChanged = QtCore.Signal(int)

    def __init__(self, parent=None):
        super(MainWidget, self).__init__(parent=parent)
        self.setObjectName("medic_main_widget")
        self.__kartes_widget = None
        self.__testers_widget = None
        self.__phase = 0
        self.__phase_widgets = {}

        self.__makeWidgets()
        self.setPhase(0)

    def phase(self):
        return self.__phase

    def next(self):
        self.setPhase(self.__phase + 1)

    def back(self):
        self.setPhase(self.__phase - 1)

    def setPhase(self, p):
        self.__phase = p
        for phase, widgets in self.__phase_widgets.iteritems():
            if phase is p:
                for widget in widgets:
                    widget.show()
            else:
                for widget in widgets:
                    widget.hide()

        if self.__phase is 0:
            able_back = False
            able_next = True if self.__kartes_widget.currentKarte() else False
            self.__testers_widget.reset()
        else:
            able_back = True
            able_next = False

        if self.__phase is 1:
            self.reset()

        self.ConditionChanged.emit(able_back, able_next)

    def __makeWidgets(self):
        main_layout = QtWidgets.QHBoxLayout()
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)
        self.setLayout(main_layout)
        self.__kartes_widget = KarteList()
        self.__testers_widget = TesterList()
        self.__detail_widget = TesterDetailWidget()

        ## phase 0
        main_layout.addWidget(self.__kartes_widget)
        self.__phase_widgets[0] = [self.__kartes_widget]

        ## phase 2
        h_layout = QtWidgets.QHBoxLayout()
        h_layout.addWidget(self.__testers_widget)
        h_layout.addWidget(self.__detail_widget)
        self.__phase_widgets[1] = [self.__testers_widget, self.__detail_widget]
        main_layout.addLayout(h_layout)

        ## signal
        self.__kartes_widget.KarteChanged.connect(self.__karteChanged)
        self.__testers_widget.TesterChanged.connect(self.__testerChanged)
        self.__detail_widget.ReportsChanged.connect(self.__reportsChanged)
        self.__testers_widget.SingleTestTriggered.connect(self.__singleTest)

    def reset(self):
        karte_item = self.__kartes_widget.currentKarte()
        if karte_item:
            karte_item.reset()
            self.StatusChanged.emit(model.Ready)
            self.update()

        tester_item = self.__testers_widget.currentTester()
        self.__detail_widget.reset()
        if tester_item:
            self.__detail_widget.setTesterItem(tester_item)

    def test(self):
        self.__detail_widget.reset()

        karte_item = self.__kartes_widget.currentKarte()
        if karte_item:
            karte_item.testAll(testerCallback=self.forceUpdate)
            self.StatusChanged.emit(karte_item.status())
            self.update()

        tester_item = self.__testers_widget.currentTester()

        if tester_item:
            self.__detail_widget.setTesterItem(tester_item)

    def __singleTest(self):
        self.__detail_widget.reset()

        karte_item = self.__kartes_widget.currentKarte()
        tester_item = self.__testers_widget.currentTester()

        if karte_item and tester_item:
            karte_item.test(tester_item, testerCallback=self.forceUpdate)
            self.StatusChanged.emit(karte_item.status())
            self.update()

            self.__detail_widget.setTesterItem(tester_item)

    def forceUpdate(self):
        self.update()
        QtWidgets.QApplication.processEvents()

    def __testerChanged(self, current):
        tester_item = self.__testers_widget.model().data(current, model.TesterItemRole)
        if tester_item:
            self.__detail_widget.setTesterItem(tester_item)
        else:
            self.__detail_widget.reset()

    def __reportsChanged(self):
        self.__testers_widget.update()
        karte_item = self.__kartes_widget.currentKarte()
        self.StatusChanged.emit(karte_item.status())

    def __karteChanged(self, current):
        able_back = False if self.__phase is 0 else True
        able_next = False

        karte_model = self.__kartes_widget.model()
        tester_model = self.__testers_widget.model()

        karte_item = karte_model.data(current, model.KarteItemRole)
        if karte_item:
            self.KarteChanged.emit(karte_item.name())
            tester_model.setTesterItems(karte_model.data(current, model.KarteTesterItemsRole))
            able_next = True
        else:
            self.KarteChanged.emit("")
            tester_model.setTesterItems([])

        self.ConditionChanged.emit(able_back, able_next)
