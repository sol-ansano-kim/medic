from maya import OpenMaya
from maya import OpenMayaUI
import Qt
from Qt import QtWidgets


BlankSelectionList = OpenMaya.MSelectionList()


def ClearSelection():
    global BlankSelectionList
    OpenMaya.MGlobal.setActiveSelectionList(BlankSelectionList)


def getMayaMainWindow():
    if Qt.IsPySide2:
        import shiboken2 as shiboken
    else:
        import shiboken

    return shiboken.wrapInstance(long(OpenMayaUI.MQtUtil_mainWindow()), QtWidgets.QMainWindow)