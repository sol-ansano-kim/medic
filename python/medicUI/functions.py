from maya import OpenMaya


BlankSelectionList = OpenMaya.MSelectionList()


def ClearSelection():
    global BlankSelectionList
    OpenMaya.MGlobal.setActiveSelectionList(BlankSelectionList)
