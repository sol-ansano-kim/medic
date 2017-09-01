from . import qtutil
from . import window
from . import functions
from maya import cmds


MedicUIInstance = None


def Show():
    global MedicUIInstance

    if MedicUIInstance:
        cmds.showWindow(window.MainWindow.Name)

    else:
        if cmds.window(window.MainWindow.Name, q=True, ex=True):
            cmds.deleteUI(window.MainWindow.Name)

        MedicUIInstance = window.MainWindow(functions.getMayaMainWindow())
        MedicUIInstance.show()
