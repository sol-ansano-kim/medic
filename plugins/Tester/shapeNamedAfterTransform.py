import medic
from maya import OpenMaya


class ShapeNamedAfterTransform(medic.PyTester):

    def __init__(self):
        super(ShapeNamedAfterTransform, self).__init__()

    def Name(self):
        return "ShapeNamedAfterTransform"

    def Description(self):
        return "Shape isn't named after the Transform"

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kShape)

    def test(self, node):
        if node.dag().isInstanced() or node.dag().isFromReferencedFile() or node.dag().isIntermediateObject():
            return None

        parent = node.parents()[0]
        if ShapeNamedAfterTransform.NameCompare(node.dg().name(), parent.dg().name()):
            return None

        return medic.PyReport(node)

    def fix(self, report, params):
        node = report.node()
        newName = ShapeNamedAfterTransform.FixName(node.parents()[0].dg().name())
        node.dg().setName(newName)
        return True

    def IsFixable(self):
        return True

    @staticmethod
    def FixName(trans):
        return "{}Shape".format(trans)

    @staticmethod
    def NameCompare(shape, trans):
        return ("{}Shape".format(trans) == shape)


def Create():
    return ShapeNamedAfterTransform()
    
