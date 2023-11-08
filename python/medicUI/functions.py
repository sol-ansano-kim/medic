from maya import OpenMaya
from maya import OpenMayaUI
from PySide2 import QtWidgets
import shiboken2
import re

FIND_PASCAL_CASE = re.compile(r'[A-Z0-9][a-z0-9]+')
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


def pascal_case_to_space_separated(string: str) -> str:
    if any(char in string for char in '_-+=/ !@#$%^&*(){}[]|\\~`:;"\'<>'):
        # This is surely not PascalCase.
        return string
    return ' '.join(FIND_PASCAL_CASE.findall(string))
