from medic.core import testerBase
from maya import OpenMaya


class EdgeLengthZero(testerBase.TesterBase):
    Name = "EdgeLengthZero"
    Description = "Zero edge length"
    ThreadsHold = 0.0000000001

    def __init__(self):
        super(EdgeLengthZero, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def Test(self, node):
        it = None
        try:
            it = OpenMaya.MItMeshEdge(node.object())
        except:
            return (False, None)

        result = False

        comp = OpenMaya.MFnSingleIndexedComponent()
        comp_obj = comp.create(OpenMaya.MFn.kMeshEdgeComponent)

        su = OpenMaya.MScriptUtil()
        dp = su.asDoublePtr()

        while (not it.isDone()):
            it.getLength(dp)
            if EdgeLengthZero.ThreadsHold > OpenMaya.MScriptUtil_getDouble(dp):
                result = True
                comp.addElement(it.index())

            it.next()

        return (result, comp_obj if result else None)


Tester = EdgeLengthZero
