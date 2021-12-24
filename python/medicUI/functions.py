from maya import OpenMaya
from maya import OpenMayaUI
from PySide2 import QtWidgets
import shiboken2
import sys


BlankSelectionList = OpenMaya.MSelectionList()
if not hasattr(__builtins__, "long"):
    long = int


def ClearSelection():
    global BlankSelectionList
    OpenMaya.MGlobal.setActiveSelectionList(BlankSelectionList)


def getMayaMainWindow():
    return shiboken2.wrapInstance(long(OpenMayaUI.MQtUtil_mainWindow()), QtWidgets.QMainWindow)


def registSceneOpenCallback(function):
    return OpenMaya.MEventMessage.addEventCallback("SceneOpened", function)


def registNewSceneOpenCallback(function):
    return OpenMaya.MEventMessage.addEventCallback("NewSceneOpened", function)


def removeCallbacks(ids):
    for id in ids:
        OpenMaya.MMessage.removeCallback(id)
