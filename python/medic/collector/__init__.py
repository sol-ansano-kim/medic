def CollectTesters():
    from . import testerCollector
    return testerCollector.GetTesters()


def CollectKartes():
    from . import karteCollector
    return karteCollector.GetKartes()