from medic.core import testerBase
from maya import OpenMaya


class LockedNormal(testerBase.TesterBase):
    Name = "LockedNormal"
    Description = "vertex(s) which has locked normal"
    Fixable = True

    def __init__(self):
        super(LockedNormal, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def Test(self, node):
        it = None
        mesh = None
        try:
            it = OpenMaya.MItMeshPolygon(node.object())
            mesh = OpenMaya.MFnMesh(node.object())
        except:
            return (False, None)

        vertices = OpenMaya.MIntArray()

        while (not it.isDone()):
            for i in range(it.polygonVertexCount()):
                vi = it.vertexIndex(i)
                if vi in vertices:
                    continue

                ni = it.normalIndex(i)
                if mesh.isNormalLocked(ni):
                    vertices.append(vi)

            it.next()

        if vertices.length() > 0:
            comp = OpenMaya.MFnSingleIndexedComponent()
            comp_obj = comp.create(OpenMaya.MFn.kMeshVertComponent)
            comp.addElements(vertices)
            return (True, comp_obj)

        return (False, None)

    def Fix(self, node, component, parameterParser):
        if node.dg().isFromReferencedFile():
            return False

        mesh = OpenMaya.MFnMesh(node.object())

        vertices = OpenMaya.MIntArray()
        ver_comp = OpenMaya.MFnSingleIndexedComponent(component)
        ver_comp.getElements(vertices)

        mesh.unlockVertexNormals(vertices)

        return True


Tester = LockedNormal
