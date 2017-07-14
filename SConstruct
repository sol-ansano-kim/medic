import excons
from excons.tools import maya
from excons.tools import python
import os
import sys


major = 1
minor = 0
patch = 0

os.environ["PYTHONPATH"] = os.environ.get("PYTHONPATH", "") + os.pathsep + os.path.abspath("cython")
maya.SetupMscver()

env = excons.MakeBaseEnv()

if sys.platform == "win32":
    excons.SetArgument("with-cython", os.path.abspath("cython/bin/cython.bat"))
else:
    excons.SetArgument("with-cython", os.path.abspath("cython/bin/cython"))

out_incdir = excons.OutputBaseDirectory() + "/include"
out_libdir = excons.OutputBaseDirectory() + "/lib"


defs = []
if sys.platform == "win32":
    cppflags = " /wd4100 /wd4505 /wd4701 /wd4127 /wd4189 /wd4005 /wd4510 /wd4512 /wd4610 /wd4211 /wd4702 /wd4706 /wd4310"
else:
    cppflags = " -Wno-unused-parameter -Wunused-variable"
prjs = []
customs = [excons.tools.maya.Require]


## cython
run_cython = False

cython_source = "src/py/_medic.pyx"
cython_out = "src/py/_medic.cpp"
if not os.path.isfile(cython_out):
    run_cython = True

else:
    out_mtime = os.stat(cython_out).st_mtime
    for pyx in excons.glob("src/py/*.p*"):
        if os.stat(pyx).st_mtime > out_mtime:
            run_cython = True
            break

if run_cython:
    print "=== generating python wrapper sources ==="
    python.CythonGenerate(env, cython_source, cpp=True, incdirs=["include"])


prjs.append({"name": "medic",
             "type": "sharedlib",
             "alias": "medic-lib",
             "defs": defs,
             "cppflags": cppflags,
             "incdirs": [out_incdir],
             "srcs": excons.glob("src/core/*.cpp"),
             "symvis": "default",
             "install": {out_incdir: excons.glob("include/*")},
             "custom": customs})

prjs.append({"name": "_medic",
             "type": "dynamicmodule",
             "alias": "medic-python",
             "ext": python.ModuleExtension(),
             "prefix": "py",
             "defs": defs,
             "rpath": out_libdir,
             "cppflags": cppflags,
             "symvis": "default",
             "incdirs": [out_incdir, "/usr/include"],
             "libdirs": [out_libdir],
             "libs": ["medic"],
             "deps": ["medic-lib"],
             "srcs": [cython_out],
             "install": {"py": ["src/py/medic.py"]},
             "custom": customs + [python.SoftRequire]})

# plugins

for plug in excons.glob("plugins/Tester/*.cpp"):
    prjs.append({"name": os.path.splitext(os.path.basename(plug))[0],
                 "type": "dynamicmodule",
                 "alias": "medic-plugins",
                 "prefix": "plugins/Tester",
                 "defs": defs,
                 "rpath": out_libdir,
                 "cppflags": cppflags,
                 "symvis": "default",
                 "incdirs": [out_incdir],
                 "libdirs": [out_libdir],
                 "libs": ["medic"],
                 "deps": ["medic-lib"],
                 "srcs": [plug],
                 "custom": customs})

py_plugs = excons.glob("plugins/Tester/*.py")
if py_plugs:
    prjs.append({"name": "medicPyPlugins",
                 "type": "install",
                 "alias": "medic-py-plugins",
                 "install": {"plugins/Tester": py_plugs}})

kartes = excons.glob("plugins/Karte/*.karte")
if kartes:
    prjs.append({"name": "medicKartes",
                 "type": "install",
                 "alias": "medic-kartes",
                 "install": {"plugins/Karte": kartes}})




excons.DeclareTargets(env, prjs)
