from medic.core import testerBase
from maya import OpenMaya


class hasLayer(testerBase.TesterBase):
    Name = "hasLayer"

    def __init__(self):
        super(hasLayer, self).__init__()

    def Match(self, m_object):
        return m_object.hasFn(OpenMaya.MFn.kDisplayLayer) or m_object.hasFn(OpenMaya.MFn.kAnimLayer) or m_object.hasFn(OpenMaya.MFn.kRenderLayer) 

    def Test(self, node):
        return node.name() not in ["BaseAnimation", "defaultRenderLayer", "defaultLayer"]


Tester = hasLayer
