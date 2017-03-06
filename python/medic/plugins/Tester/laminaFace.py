from medic.core import testerBase
from maya import OpenMaya


class LaminaFace(testerBase.TesterBase):
    Name = "LaminaFace"

    def __init__(self):
        super(LaminaFace, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def Test(self, node):
        it = None
        try:
            it = OpenMaya.MItMeshPolygon(node.object())
        except:
            return (False, None)

        result = False

        comp = OpenMaya.MFnSingleIndexedComponent()
        comp_obj = comp.create(OpenMaya.MFn.kMeshPolygonComponent)

        while (not it.isDone()):
            if it.isLamina():
                comp.addElement(it.index())
                result = True

            it.next()

        return (result, comp_obj if result else None)


Tester = LaminaFace
