from PySide2 import QtWidgets, QtCore
from maya import cmds
from . import widgets
from . import functions
import os
import json
import string


class MainWindow(QtWidgets.QMainWindow):
    Name = "medic_main_window"
    Title = "MEDIC"

    def __init__(self, visitorClass=None, parent=None, topMost=False):
        super(MainWindow, self).__init__(parent=parent)
        self.__visitor_class = visitorClass
        self.setObjectName(MainWindow.Name)
        self.setWindowTitle(MainWindow.Title)
        if topMost:
            self.setWindowFlags(self.windowFlags() | QtCore.Qt.WindowStaysOnTopHint)
        self.__makeWidgets()
        self.__setStyleSheet()
        self.__registJobs()

    def setKarte(self, karteName):
        if self.__main_widget.setKarte(karteName):
            self.__top_widget.setCurrentKarteName(karteName)
            self.__top_widget.setBrowserButtonEnabled(False, True)
            self.__next()
            return True
        else:
            self.__top_widget.setCurrentKarteName("")
            self.__top_widget.setBrowserButtonEnabled(False, False)
            return False

    def runSingle(self, karteName, testerName):
        if self.setKarte(karteName):
            return self.__main_widget.runSingle(testerName)
        else:
            return None

    def showAllKartes(self, v):
        self.__main_widget.showAllKartes(v)

    def __setStyleSheet(self):
        qss_path = os.path.abspath(os.path.join(__file__, "../style.qss"))
        scale_style_path = os.path.abspath(os.path.join(__file__, "../scale_style.json"))

        if not os.path.isfile(qss_path) or not os.path.isfile(scale_style_path):
            return

        current_dir = os.path.dirname(__file__)

        with open(scale_style_path, "r") as f:
            scale_style_data = json.load(f)
            scale_style_data = {k: functions.applyMonitorScalingTo(v)
                                for k, v in scale_style_data.items()}

        with open(qss_path, "r") as f:
            template = string.Template(f.read())
            style = template.safe_substitute(scale_style_data)
            style = style.replace('url("', 'url("%s/' % current_dir.replace("\\", "/"))

        self.setStyleSheet(style)

    def __makeWidgets(self):
        self.setMinimumWidth(450)
        self.setMinimumHeight(300)
        self.__central_widget = QtWidgets.QWidget()
        self.__central_layout = QtWidgets.QVBoxLayout()
        self.setCentralWidget(self.__central_widget)
        self.__central_widget.setLayout(self.__central_layout)

        self.__top_widget = widgets.TopBarWidget()
        self.__main_widget = widgets.MainWidget(visitorClass=self.__visitor_class)

        self.__central_layout.addWidget(self.__top_widget)
        self.__central_layout.addWidget(self.__main_widget)
        self.__central_layout.setSpacing(0)
        self.__central_layout.setContentsMargins(0, 0, 0, 0)

        self.__top_widget.setBrowserButtonEnabled(False, False)
        self.__main_widget.ConditionChanged.connect(self.__top_widget.setBrowserButtonEnabled)
        self.__main_widget.KarteChanged.connect(self.__top_widget.setCurrentKarteName)
        self.__top_widget.BackClicked.connect(self.__back)
        self.__top_widget.NextClicked.connect(self.__next)
        self.__top_widget.SelectionOnlyChanged.connect(self.__main_widget.setSelectionOnly)
        self.__top_widget.reset_button.clicked.connect(self.__main_widget.reset)
        self.__top_widget.test_button.clicked.connect(self.__main_widget.test)
        self.__main_widget.StatusChanged.connect(self.__top_widget.status_label.setStatus)

    def __registJobs(self):
        cmds.scriptJob(event=('NewSceneOpened', self.__main_widget.reset), parent=self.objectName())
        cmds.scriptJob(event=('SceneOpened', self.__main_widget.reset), parent=self.objectName())

    def __next(self):
        self.__top_widget.next()
        self.__main_widget.next()

    def __back(self):
        self.__top_widget.back()
        self.__main_widget.back()
