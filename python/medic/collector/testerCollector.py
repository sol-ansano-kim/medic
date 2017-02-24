import os
import imp
import re
from ..core import testerBase
import copy


PLUGIN_BASE_CLASS = testerBase.TesterBase
PLUGIN_PATH_ENV = "MEDIC_TESTER_PATH"
RE_EXT_PY = re.compile("[.]py$", re.IGNORECASE)
BUILT_IN_DIR_PATH = os.path.abspath(os.path.join(__file__, "../../plugins/Tester")).replace("\\", "/")

_SEARCHED_TESTERS = []


def _importModule(path):
    module = None

    try:
        module = imp.load_source(os.path.splitext(os.path.basename(path))[0], path)

    except Exception as e:
        return None

    return module


def _loadTesters():
    global _SEARCHED_TESTERS

    _SEARCHED_TESTERS = []
    search_dirs = map(lambda x : os.path.abspath(x), filter(lambda y : y, os.environ.get(PLUGIN_PATH_ENV, "").split(os.pathsep)))
    search_dirs.append(BUILT_IN_DIR_PATH)

    for sdir in search_dirs:
        if not os.path.isdir(sdir):
            continue

        for f in os.listdir(sdir):
            module = None
            full_path = os.path.join(sdir, f).replace("\\", "/")
            not_keys = []

            if not RE_EXT_PY.search(f):
                continue

            module = _importModule(full_path)

            if not module:
                continue

            if not hasattr(module, "Tester"):
                continue

            if not hasattr(module.Tester, "Name"):
                continue

            if not issubclass(module.Tester, PLUGIN_BASE_CLASS):
                continue

            _SEARCHED_TESTERS.append(module.Tester)


def GetTesters(forceReload=False):
    if forceReload or not _SEARCHED_TESTERS:
        _loadTesters()

    return copy.copy(_SEARCHED_TESTERS)
