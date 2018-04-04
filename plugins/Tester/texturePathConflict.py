import medic
from maya import OpenMaya
from maya import cmds

class TexturePathConflict(medic.PyTester):

    def __init__(self):
        super(TexturePathConflict, self).__init__()

    def initialize(self):
        self.fileNodes = {}
        for f in cmds.ls(type="file"):
            self.fileNodes[f] = (cmds.getAttr("{}.fileTextureName".format(f)), 
                                 cmds.getAttr("{}.useFrameExtension".format(f)))

    def Name(self):
        return "TexturePathConflict"

    def Description(self):
        return "'Image Name' matches. 'Use Image Sequence' not match."

    def Match(self, node):
        return node.object().hasFn(OpenMaya.MFn.kFileTexture)

    def IsFixable(self):
        return False

    def test(self, node):
        if self.checkConflicts(node):
            return medic.PyReport(node)
        return None

    def checkConflicts(self, node):
        ftn, ext = self.fileNodes.get(node.name())
        for key, values in self.fileNodes.iteritems():
            if key ==node.name():
                continue
            if ftn != values[0]:
                #filename compare
                continue
            if ext != values[1]:
                #useImageSequence
                return True
        return False

def Create():
    return TexturePathConflict()
