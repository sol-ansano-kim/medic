from . import qtutil
from . import window
from . import functions
from maya import cmds


MedicUIInstance = None


def Show(visitorClass=None, karteName=None, showAllKartes=False, parent=None):
    global MedicUIInstance

    if cmds.window(window.MainWindow.Name, q=True, ex=True):
        cmds.deleteUI(window.MainWindow.Name)

    if parent is None:
        parent = functions.getMayaMainWindow()

    MedicUIInstance = window.MainWindow(visitorClass=visitorClass, parent=parent)
    MedicUIInstance.show()

    MedicUIInstance.showAllKartes(showAllKartes)

    if karteName:
        MedicUIInstance.setKarte(karteName)

    return MedicUIInstance
