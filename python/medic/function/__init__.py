from .. import collector


def GetKartes(forceReload=False):
    return collector.GetKartes(forceReload=forceReload)


def GetKarte(karte_name, forceReload=False):
    kartes = collector.GetKartes(forceReload=forceReload)
    for karte in kartes:
        if karte.name() == karte_name:
            return karte

    return None


def Visit(karte, visitor):
    karte.accept(visitor)
