import sys
import os


sys.path.append(os.path.abspath("debug/py"))
sys.path.append(os.path.abspath("release/py"))

os.environ["MEDIC_KARTE_PATH"] = os.path.abspath(os.path.join(__file__, "../kartes"))
tester_path = []
tester_path.append(os.path.abspath(os.path.join(__file__, "../testers")))
tester_path.append(os.path.abspath(os.path.join(__file__, "../../debug/test/testers")))
tester_path.append(os.path.abspath(os.path.join(__file__, "../../release/test/testers")))

os.environ["MEDIC_TESTER_PATH"] = os.pathsep.join(tester_path)


import _medic


node1 = _medic.Node()
node2 = _medic.Node()


class NewVisitor(_medic.Visitor):
    def __init__(self, manager):
        super(NewVisitor, self).__init__(manager)

    def __collectNodes(self):
        print "TEST"


def test():
    manager = _medic.PluginManager.Instance()
    kartes = manager.getKarteNames()
    testers = manager.getTesterNames()
    if not testers:
        print("No tester")
        sys.exit(1)
    
    print("Found testers : %s" % testers)

    if not kartes:
        print("No Karte")
        sys.exit(1)

    print("Found kartes : %s" % kartes)
    visitor = NewVisitor(manager)

    for karte in kartes:
        print("Set Karte : %s" % karte)
        if not visitor.setKarte(manager.getKarte(karte)):
            print("!! Failed to set karte : %s" % karte)    
            sys.exit(1)

        print("Test All")
        visitor.testAll()    
    
    sys.exit(0)

if __name__ == "__main__":
    test()
