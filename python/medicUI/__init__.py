from . import qtutil
from . import window
from . import functions
from maya import cmds


MedicUIInstance = None


def Show(visitorClass=None, karteName=None, showAllKartes=False):
    global MedicUIInstance

    if MedicUIInstance:
        cmds.showWindow(window.MainWindow.Name)

    else:
        if cmds.window(window.MainWindow.Name, q=True, ex=True):
            cmds.deleteUI(window.MainWindow.Name)

        MedicUIInstance = window.MainWindow(visitorClass=visitorClass, parent=functions.getMayaMainWindow())
        MedicUIInstance.show()

    if karteName:
        MedicUIInstance.setKarte(karteName)
    MedicUIInstance.showAllKartes(showAllKartes)
