def GetTesters(forceReload=False):
    from . import testerCollector
    return testerCollector.GetTesters(forceReload=forceReload)


def GetKartes(forceReload=False):
    from . import karteCollector
    return karteCollector.GetKartes(forceReload=forceReload)
