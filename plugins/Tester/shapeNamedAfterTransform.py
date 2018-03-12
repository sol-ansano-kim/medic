import medic
from maya import OpenMaya
import re
from maya import cmds


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

        for p in node.parents():
            correctShapeName = self.NameCompare(node.name(), p.name())
            if correctShapeName:
                return None

        return medic.PyReport(node)

    def fix(self, report, params):
        node = report.node()
        newName= self.NameCompare(node.name(), node.parents()[0].name(), fixMe=True)
        cmds.rename(node.name(), newName)
        return True

    def IsFixable(self):
        return True

    def NameCompare(self, shape, trans, fixMe=False):
    
        tmp_trans = trans.split('|')[-1]
        tmp_shapeName = tmp_trans + "Shape"
        shape_lastName = shape.split("|")[-1]

        if fixMe:
            return shape.replace(shape_lastName, tmp_shapeName)

        if tmp_shapeName != shape_lastName:
            return False
        else:
            return True

def Create():
    return ShapeNamedAfterTransform()
    
