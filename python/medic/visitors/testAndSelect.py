from . import testOnly
from maya import OpenMaya


class TestAndSelect(testOnly.TestOnly):
    def __init__(self):
        super(TestAndSelect, self).__init__()

    def __addSelection(self, selection_list, node, component):
        if node.IsDagNode():
            if component is not None:
                selection_list.add(node.getPath(), component)
            else:
                selection_list.add(node.getPath())
        else:
            selection_list.add(node.object())


    def Select(self, tester_name):
        res = self.result(tester_name)

        sel = OpenMaya.MSelectionList()
        for node, comp in res:
            self.__addSelection(sel, node, comp)

        OpenMaya.MGlobal.setActiveSelectionList(sel)

    def SelectAll(self):
        results = self.results()

        sel = OpenMaya.MSelectionList()
        for res in results.values():
            for node, comp in res:
                self.__addSelection(sel, node, comp)

        OpenMaya.MGlobal.setActiveSelectionList(sel)
