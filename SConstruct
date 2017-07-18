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


defs = []
if sys.platform == "win32":
    cppflags = " /wd4100 /wd4505 /wd4701 /wd4127 /wd4189 /wd4005 /wd4510 /wd4512 /wd4610 /wd4211 /wd4702 /wd4706 /wd4310"
else:
    cppflags = " -Wno-unused-parameter -Wunused-variable"
prjs = []
customs = [excons.tools.maya.Require]


mayaver = excons.GetArgument("mayaver", None)
if not mayaver:
    mayaver = excons.GetArgument("with-maya", None)

try:
    float(mayaver)
except:
    print "ERROR : Set Maya version with 'mayaver'"
    sys.exit(1)


out_incdir = excons.OutputBaseDirectory() + "/include"
out_libdir = excons.OutputBaseDirectory() + "/lib/" + mayaver
out_pydir = excons.OutputBaseDirectory() + "/py/" + mayaver

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


## custom plugins
custom_plugins = excons.GetArgument("plugin-path", None)
custom_cpp = []
custom_py = []

if custom_plugins:
    custom_cpp = excons.glob(os.path.join(custom_plugins, "*.cpp"))
    custom_py = excons.glob(os.path.join(custom_plugins, "*.py"))


## build
headers = excons.glob("include/*")


prjs.append({"name": "medic",
             "type": "sharedlib",
             "alias": "medic-lib",
             "defs": defs,
             "cppflags": cppflags,
             "incdirs": [out_incdir],
             "prefix": mayaver,
             "srcs": excons.glob("src/core/*.cpp"),
             "symvis": "default",
             "install": {out_incdir: headers},
             "custom": customs})

prjs.append({"name": "_medic",
             "type": "dynamicmodule",
             "alias": "medic-python",
             "ext": python.ModuleExtension(),
             "prefix": "py/%s" % (mayaver),
             "defs": defs,
             "rpath": out_libdir,
             "cppflags": cppflags,
             "symvis": "default",
             "incdirs": [out_incdir, "/usr/include"],
             "libdirs": [out_libdir],
             "libs": ["medic"],
             "deps": ["medic-lib"],
             "srcs": [cython_out],
             "install": {out_pydir: ["src/py/medic.py"]},
             "custom": customs + [python.SoftRequire]})

# plugins

for plug in excons.glob("plugins/Tester/*.cpp"):
    prjs.append({"name": os.path.splitext(os.path.basename(plug))[0],
                 "type": "dynamicmodule",
                 "alias": "medic-plugins",
                 "prefix": "plugins/%s/Tester" % (mayaver),
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
                 "install": {"plugins/%s/Tester" % (mayaver): py_plugs}})

kartes = excons.glob("plugins/Karte/*.karte")
if kartes:
    prjs.append({"name": "medicKartes",
                 "type": "install",
                 "alias": "medic-kartes",
                 "install": {"plugins/%s/Karte" % (mayaver): kartes}})

if custom_cpp:
    for cpp in custom_cpp:
        prjs.append({"name": os.path.splitext(os.path.basename(cpp))[0],
                     "type": "dynamicmodule",
                     "alias": "medic-custom-plugins",
                     "prefix": "custom/%s/Tester" % (mayaver),
                     "defs": defs,
                     "rpath": out_libdir,
                     "cppflags": cppflags,
                     "symvis": "default",
                     "incdirs": [out_incdir],
                     "libdirs": [out_libdir],
                     "libs": ["medic"],
                     "deps": ["medic-lib"],
                     "srcs": [cpp],
                     "custom": customs})

if custom_py:
    prjs.append({"name": "medicCustonPyPlugins",
                 "type": "install",
                 "alias": "medic-custom-py-plugins",
                 "install": {"custom/%s/Tester" % (mayaver): custom_py}})



excons.DeclareTargets(env, prjs)
