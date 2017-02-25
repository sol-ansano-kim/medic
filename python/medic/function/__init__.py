from .. import collector


def GetKartes(forceReload=False):
    return collector.GetKartes(forceReload=forceReload)


def GetKarte(karte_name):
    kartes = collector.GetKartes()
    for karte in kartes:
        if karte.name() == karte_name:
            return karte

    return None


def GetNodeTester():
    from ..visitors import testNodes
    return testNodes.TestNodes()


def Visit(karte, visitor):
    karte.accept(visitor)


def Result(karte):
    return karte.results()