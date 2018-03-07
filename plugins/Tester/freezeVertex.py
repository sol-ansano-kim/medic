import medic
from maya import OpenMaya
from maya import cmds
from pprint import pprint
import numpy as np
import sys


class FreezeVertex(medic.PyTester):
    def __init__(self):
        super(FreezeVertex, self).__init__()

    def Name(self):
        return "FreezeVertex"

    def Description(self):
        return "Not Frozen Verticies"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh) or node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom)

    def test(self, node, geom = None):
        try:
            if node.object().hasFn(OpenMaya.MFn.kMesh):
                geom = OpenMaya.MFnMesh(node.object())
            elif node.object().hasFn(OpenMaya.MFn.kNurbsSurfaceGeom):
                geom = OpenMaya.MFnNurbsSurface(node.object())
        except:
            return None

        meshVtx = geom.numVertices()
        vCount = OpenMaya.MIntArray()
        vList = OpenMaya.MIntArray()
        plug = node.dg().findPlug("pnts", True)
        tweaked = False
        for i in range(plug.numElements()):
            elm = plug.elementByPhysicalIndex(i)
            for j in range(3):
                if abs(elm.child(j).asDouble()) > sys.float_info.epsilon:
                    tweaked = True
                    break
            if tweaked:
                break
            i+=1
        if tweaked:
            print "tweaked"
            return medic.PyReport(node)
        else:
            print "clear"
            return None

    def IsFixable(self):
        return True

    def fix(self, report, params):
        node    = report.node()
        ori     = cmds.ls(sl=True)
        objList = cmds.ls(node.name())
        print len(objList)
        if not objList or len(objList) < 1:
            print("[Reset Vertex Transform] No node is selected.")
            return None
        
        for obj in objList:
            cmds.cluster(obj)
            cmds.delete(obj, ch=True)
            print("[Reset Vertex Transform] reset VT")

        if ori:
            cmds.select(ori, r=True)

        print("[Reset Vertex Transform] OK.")
        return True

def Create():
    return FreezeVertex()