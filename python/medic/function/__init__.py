from .. import collector
from ..core import nodes


def GetKartes(forceReload=False):
    return collector.GetKartes(forceReload=forceReload)


def GetKarte(karte_name, forceReload=False):
    kartes = collector.GetKartes(forceReload=forceReload)
    for karte in kartes:
        if karte.name() == karte_name:
            return karte

    return None


def GetTesters(forceReload=False):
    return collector.GetTesters(forceReload=forceReload)


def GetAllNodes():
    return nodes.GetNodes()


def Visit(karte, visitor):
    karte.accept(visitor)


def Report(karte_name):
    from ..visitors import testAndReport
    karte = GetKarte(karte_name)
    if not karte:
        print "Warning : no karte '%s'" % (karte_name)
        return {}

    visitor = testAndReport.TestAndReport()
    Visit(karte, visitor)

    return visitor.Report()
