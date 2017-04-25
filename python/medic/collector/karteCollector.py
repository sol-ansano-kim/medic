import os
import re
import copy
from ..core import karte


PLUGIN_PATH_ENV = "MEDIC_KARTE_PATH"
RE_EXT_KARTE = re.compile("[.]karte$", re.IGNORECASE)
BUILT_IN_DIR_PATH = os.path.abspath(os.path.join(__file__, "../../plugins/Karte")).replace("\\", "/")

_SEARCHED_KARTES = []


def _importKarte(path, testers):
    karte_data = {}

    try:
        with open(path, "r") as f:
            karte_data = eval(f.read())

            if karte_data:
                k = karte.Karte(karte_data, path, testers)
                return k
    except:
        return None

    return None


def _loadKartes():
    from . import testerCollector
    testers = testerCollector.GetTesters(forceReload=True)

    global _SEARCHED_KARTES

    _SEARCHED_KARTES = []
    search_dirs = map(lambda x : os.path.abspath(x), filter(lambda y : y, os.environ.get(PLUGIN_PATH_ENV, "").split(os.pathsep)))
    search_dirs.append(BUILT_IN_DIR_PATH)

    for sdir in search_dirs:
        if not os.path.isdir(sdir):
            continue

        for f in os.listdir(sdir):
            module = None
            full_path = os.path.join(sdir, f).replace("\\", "/")
            not_keys = []

            if not RE_EXT_KARTE.search(f):
                continue

            karte = _importKarte(full_path, testers)

            if not karte:
                continue

            _SEARCHED_KARTES.append(karte)


def GetKartes(forceReload=False):
    if forceReload or not _SEARCHED_KARTES:
        _loadKartes()

    return copy.copy(_SEARCHED_KARTES)
