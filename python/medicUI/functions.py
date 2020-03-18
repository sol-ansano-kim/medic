from maya import OpenMaya
from maya import OpenMayaUI
import Qt
from Qt import QtWidgets


BlankSelectionList = OpenMaya.MSelectionList()


def ClearSelection():
    global BlankSelectionList
    OpenMaya.MGlobal.setActiveSelectionList(BlankSelectionList)


def getMayaMainWindow():
    if hasattr(Qt, "IsPySide2"):
        if Qt.IsPySide2:
            import shiboken2 as shiboken
        else:
            import shiboken

    # Qt version less than 1.0.0
    elif hasattr(Qt, "__version_info__"):
        if Qt.__version_info__[0] >= 2:
            import shiboken2 as shiboken
        else:
            import shiboken

    else:
        try:
            import shiboken2 as shiboken
        except:
            import shiboken

    return shiboken.wrapInstance(long(OpenMayaUI.MQtUtil_mainWindow()), QtWidgets.QMainWindow)


def registSceneOpenCallback(function):
    return OpenMaya.MEventMessage.addEventCallback("SceneOpened", function)


def registNewSceneOpenCallback(function):
    return OpenMaya.MEventMessage.addEventCallback("NewSceneOpened", function)


def removeCallbacks(ids):
    for id in ids:
        OpenMaya.MMessage.removeCallback(id)
