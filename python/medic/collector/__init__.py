def CollectTesters(forceReload=False):
    from . import testerCollector
    return testerCollector.GetTesters(forceReload=forceReload)


def CollectKartes(forceReload=False):
    from . import karteCollector
    return karteCollector.GetKartes(forceReload=forceReload)
