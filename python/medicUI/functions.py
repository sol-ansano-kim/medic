from maya import OpenMaya
from maya import OpenMayaUI
from PySide2 import QtWidgets
from PySide2.QtGui import QGuiApplication
import shiboken2

BlankSelectionList = OpenMaya.MSelectionList()
if not hasattr(__builtins__, "long"):
    long = int


def ClearSelection():
    global BlankSelectionList
    OpenMaya.MGlobal.setActiveSelectionList(BlankSelectionList)


def getMayaMainWindow():
    return shiboken2.wrapInstance(long(OpenMayaUI.MQtUtil.mainWindow()), QtWidgets.QMainWindow)


def registSceneOpenCallback(function):
    return OpenMaya.MEventMessage.addEventCallback("SceneOpened", function)


def registNewSceneOpenCallback(function):
    return OpenMaya.MEventMessage.addEventCallback("NewSceneOpened", function)


def removeCallbacks(ids):
    for id in ids:
        OpenMaya.MMessage.removeCallback(id)


def getMoniterScalingFactor() -> float:
    screen = QGuiApplication.primaryScreen()
    if screen:
        return screen.logicalDotsPerInch() / 96.0
    return 1.0


MONITOR_SCALING_FACTOR = getMoniterScalingFactor()


def applyMonitorScalingTo(value: int | float) -> int:
    return int(value * MONITOR_SCALING_FACTOR)
