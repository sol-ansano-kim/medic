from . import dependencyNode
from maya import OpenMaya


class DagNode(dependencyNode.DependencyNode):
    FuncionType = OpenMaya.MFn.kDagNode
    Name = "Dag"

    def __init__(self, m_object):
        super(DagNode, self).__init__(m_object)
        self._mfn_dag = OpenMaya.MFnDagNode(self._m_object)

    def dag(self):
        return self._mfn_dag

    def name(self):
        return self.dag().partialPathName()

    def parents(self):
        prnts = []

        dag = self.dag()
        if dag is None:
            return []

        for i in range(dag.parentCount()):
            p_dag = DagNode(dag.parent(i))

            if p_dag.name() == "":
                continue

            prnts.append(p_dag)

        return prnts

    def children(self):
        chdrn = []

        dag = self.dag()
        if dag is None:
            return []

        for i in range(dag.childCount()):
            chdrn.append(DagNode(dag.child(i)))

        return chdrn
