from maya import OpenMaya


class DependencyNode(object):
    FuncionType = OpenMaya.MFn.kDependencyNode
    Name = "Dependency"

    @classmethod
    def Create(klass, m_object):
        if not klass.Match(m_object):
            return None

        return klass(m_object)

    @classmethod
    def Match(klass, m_object):
        if m_object.isNull():
            return False

        if not m_object.hasFn(klass.FuncionType):
            return False

        return True

    def __repr__(self):
        return "<%s '%s'>" % (self.__class__.__name__, self.name())

    def __init__(self, m_object):
        super(DependencyNode, self).__init__()
        self._m_object = m_object
        self._mfn_depn = OpenMaya.MFnDependencyNode(self._m_object)

    def object(self):
        return self._m_object

    def dg(self):
        return self._mfn_depn

    def name(self):
        return self.dg().name()

    def type(self):
        return self.dg().typeName()
