from medic.core import testerBase
from maya import OpenMaya
import sys


class EdgeLengthZero(testerBase.TesterBase):
    Name = "EdgeLengthZero"
    ThreadsHold = 0.001

    def __init__(self):
        super(EdgeLengthZero, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def Test(self, node):
        if node.object().hasFn(OpenMaya.MFn.kMesh):
            it = None
            try:
                it = OpenMaya.MItMeshEdge(node.object())
            except:
                return (False, None)

            su = OpenMaya.MScriptUtil()
            dp = su.asDoublePtr()

            while (not it.isDone()):
                it.getLength(dp)
                if EdgeLengthZero.ThreadsHold > OpenMaya.MScriptUtil_getDouble(dp):
                    return (True, None)

                it.next()

        return (False, None)


Tester = EdgeLengthZero
