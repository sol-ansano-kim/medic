from medic.core import testerBase
from maya import OpenMaya


class NonManifold(testerBase.TesterBase):
    Name = "NonManifold"
    Description = "Non manifold mesh(s) exists"

    def __init__(self):
        super(NonManifold, self).__init__()

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

        while (not it.isDone()):
            faces = OpenMaya.MIntArray()
            if it.getConnectedFaces(faces) > 2:
                result = True
                comp.addElement(it.index())

            it.next()

        return (result, comp_obj if result else None)


Tester = NonManifold
