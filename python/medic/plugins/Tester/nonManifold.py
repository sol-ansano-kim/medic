from medic.core import testerBase
from maya import OpenMaya


class NonManifold(testerBase.TesterBase):
    Name = "NonManifold"

    def __init__(self):
        super(NonManifold, self).__init__()

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kMesh)

    def Test(self, node):
        it = None
        try:
            it = OpenMaya.MItMeshPolygon(node.object())
        except:
            return False

        all_edges = []

        while (not it.isDone()):
            edges = OpenMaya.MIntArray()

            it.getEdges(edges)
            edge_set = set(edges)

            if edge_set in all_edges:
                return True

            all_edges.append(edge_set)

            it.next()

        return False


Tester = NonManifold
