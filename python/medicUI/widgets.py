from Qt import QtWidgets, QtCore, QtGui
from . import model
from . import delegate
import os


IconDir = os.path.abspath(os.path.join(__file__, "../icons"))


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
        self.setFixedWidth(50)
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

    def mousePressEvent(self, evnt):
        if QtCore.Qt.MouseButton.LeftButton == evnt.button():
            index = self.indexAt(evnt.pos())
            if index.row() < 0:
                self.clearSelection()
                self.__current_karte = None
            else:
                self.__current_karte = self.source_model.data(index, model.KarteItemRole)

            self.KarteChanged.emit(index)
            super(KarteList, self).mousePressEvent(evnt)


class TesterList(QtWidgets.QListView):
    def __init__(self, parent=None):
        super(TesterList, self).__init__(parent=parent)
        self.setObjectName("medic_tester_list")
        self.setAlternatingRowColors(True)
        self.setUniformItemSizes(True)
        self.source_model = model.TesterModel()
        self.delegate = delegate.TesterDelegate()
        self.setItemDelegate(self.delegate)
        self.setModel(self.source_model)


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

        ## phase 0
        main_layout.addWidget(self.__kartes_widget)
        self.__phase_widgets[0] = [self.__kartes_widget]

        ## phase 2
        v_layout = QtWidgets.QHBoxLayout()
        v_layout.addWidget(self.__testers_widget)
        self.__phase_widgets[1] = [self.__testers_widget]
        main_layout.addLayout(v_layout)

        ## signal
        self.__kartes_widget.KarteChanged.connect(self.__karteChanged)

    def reset(self):
        karte_item = self.__kartes_widget.currentKarte()
        if karte_item:
            karte_item.reset()
            self.StatusChanged.emit(model.Ready)
            self.update()

    def test(self):
        karte_item = self.__kartes_widget.currentKarte()
        if karte_item:
            karte_item.testAll()
            self.StatusChanged.emit(karte_item.status())
            self.update()

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
