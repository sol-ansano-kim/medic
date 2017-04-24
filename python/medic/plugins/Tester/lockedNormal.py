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
            it = OpenMaya.MItMeshVertex(node.object())
            mesh = OpenMaya.MFnMesh(node.object())
        except:
            return (False, None)

        result = False

        comp = OpenMaya.MFnSingleIndexedComponent()
        comp_obj = comp.create(OpenMaya.MFn.kMeshVertComponent)

        while (not it.isDone()):
            normal_indices = OpenMaya.MIntArray()
            it.getNormalIndices(normal_indices)

            for i in range(normal_indices.length()):
                if mesh.isNormalLocked(normal_indices[i]):
                    result = True
                    comp.addElement(it.index())
                    break

            it.next()

        return (result, comp_obj if result else None)

    def Fix(self, node, component, parameterParser):
        if node.dg().isFromReferencedFile():
            return False

        target_normal_indices = OpenMaya.MIntArray()

        mesh = OpenMaya.MFnMesh(node.object())
        it = OpenMaya.MItMeshVertex(node.getPath(), component)
        while (not it.isDone()):
            normal_indices = OpenMaya.MIntArray()
            it.getNormalIndices(normal_indices)

            for i in range(normal_indices.length()):
                target_normal_indices.append(normal_indices[i])
            it.next()

        mesh.unlockVertexNormals(target_normal_indices)

        return True


Tester = LockedNormal
