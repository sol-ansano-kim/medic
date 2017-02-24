from . import dependencyNode
from . import dagNode
from maya import OpenMaya


def GetNodes():
    result = []
    it = OpenMaya.MItDependencyNodes()

    deps = []
    while (not it.isDone()):
        obj = it.thisNode()
        dep = dependencyNode.DependencyNode(obj)
        dg = dep.dg()

        if not dg.isShared() and not dg.isDefaultNode() and dg.canBeWritten() and not dg.isFromReferencedFile():
            deps.append(dep)

        it.next()

    for dep in deps:
        if dagNode.DagNode.Match(dep.object()):
            result.append(dagNode.DagNode(dep.object()))

        else:
            result.append(dep)

    return result
