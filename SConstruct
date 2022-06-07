import excons
from excons.tools import maya
from excons.tools import python
import os
import sys
import shutil
import re
import SCons

major = 1
minor = 5
patch = 4

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
    if maya.Version(asString=False, nice=True) >= 2018:
        cppflags += " -std=c++11"

prjs = []
customs = [excons.tools.maya.Require]

mayaver = excons.GetArgument("with-maya", None)

try:
    float(mayaver)
except:
    print("ERROR : Set Maya version with 'with-maya'")
    sys.exit(1)


out_incdir = excons.OutputBaseDirectory() + "/include"
out_libdir = excons.OutputBaseDirectory() + "/lib/" + mayaver
out_pydir = excons.OutputBaseDirectory() + "/py/" + mayaver
os_name = "linux"
if sys.platform == "win32":
    os_name = "windows"
elif sys.platform == "darwin":
    os_name = "macOS"

if mayaver in ("2022", ):
    install_dir = "%s/dist/medic_%s_%s_py%s/medic" % (excons.OutputBaseDirectory(), os_name, mayaver, python.Version().replace(".", ""))
    package_file = "%s/dist/medic_%s_maya%s-py%s-%s_%s_%s.zip" % (excons.OutputBaseDirectory(), os_name, mayaver, python.Version().replace(".", ""), major, minor, patch)
else:
    install_dir = "%s/dist/medic_%s_%s/medic" % (excons.OutputBaseDirectory(), os_name, mayaver)
    package_file = "%s/dist/medic_%s_maya%s-%s_%s_%s.zip" % (excons.OutputBaseDirectory(), os_name, mayaver, major, minor, patch)


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
    python.RequireCython(env)
    print("=== generating python wrapper sources ===")
    python.CythonGenerate(env, cython_source, cpp=True, incdirs=["include"])


## custom plugins
custom_plugins = excons.GetArgument("plugin-path", None)
custom_cpp = []
custom_py = []

if custom_plugins:
    custom_cpp = excons.glob(os.path.join(custom_plugins, "*.cpp"))
    custom_py = excons.glob(os.path.join(custom_plugins, "*.py"))


## build
headers = excons.glob("include/medic/*")


def Pacakage(target, source, env):
    path = target[0].get_path()
    path_split = re.split(r"[\\/]", path)

    dirname = install_dir

    if "include" in path_split:
        dirname = os.path.join(install_dir, "include/medic")
    elif "lib" in path_split:
        dirname = os.path.join(install_dir, "lib")
    elif "bin" in path_split:
        dirname = os.path.join(install_dir, "bin")
    elif "Karte" in path_split:
        dirname = os.path.join(install_dir, "plugins/Karte")
    elif "Tester" in path_split:
        dirname = os.path.join(install_dir, "plugins/Tester")
    elif "py" in path_split:
        dirname = os.path.join(install_dir, "py")

    if not os.path.isdir(dirname):
        os.makedirs(dirname)

    out_path = os.path.join(dirname, os.path.basename(path))
    if not os.path.isfile(out_path):
        print("Package : %s -> %s" % (path, out_path))
        shutil.copy(path, out_path)

prjs.append({"name": "medic",
             "type": "sharedlib",
             "alias": "medic-lib",
             "defs": defs,
             "cppflags": cppflags,
             "incdirs": [out_incdir],
             "prefix": mayaver,
             "bldprefix": "maya-%s" % mayaver,
             "srcs": excons.glob("src/core/*.cpp"),
             "symvis": "default",
             "install": {out_incdir + "/medic": headers},
             "custom": customs})

prjs.append({"name": "_medic",
             "type": "dynamicmodule",
             "alias": "medic-python",
             "ext": python.ModuleExtension(),
             "prefix": "py/%s" % (mayaver),
             "bldprefix": "maya-%s" % mayaver,
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
                 "bldprefix": "maya-%s" % mayaver,
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
    prjs.append({"name": "pyPlugins",
                 "type": "install",
                 "alias": "medic-py-plugins",
                 "install": {"plugins/%s/Tester" % (mayaver): py_plugs}})

kartes = excons.glob("plugins/Karte/*.karte")
if kartes:
    prjs.append({"name": "kartes",
                 "type": "install",
                 "alias": "medic-kartes",
                 "install": {"plugins/%s/Karte" % (mayaver): kartes}})

if custom_cpp:
    for cpp in custom_cpp:
        prjs.append({"name": os.path.splitext(os.path.basename(cpp))[0],
                     "type": "dynamicmodule",
                     "alias": "medic-custom-plugins",
                     "prefix": "custom/%s/Tester" % (mayaver),
                     "bldprefix": "maya-%s" % mayaver,
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

prjs.append({"name": "medicUI",
             "type": "install",
             "alias": "medic-ui",
             "install": {out_pydir: ["python/medicUI"]}})


targets = excons.DeclareTargets(env, prjs)

env.Alias("medicAll", targets.keys())


def __getAllPaths(c):
    if isinstance(c, str):
        return [c]

    if isinstance(c, SCons.Node.FS.File):
        return [c.get_path()]

    paths = []
    if isinstance(c, SCons.Node.FS.Entry):
        for child in c.all_children():
            paths += __getAllPaths(child)

    return paths


env.Alias("install", env.Install(os.path.join(install_dir, ".."), "medic.mod"))
env.Alias("install", env.Install(os.path.join(install_dir, "py"), "src/py/medic.py"))

for h in headers:
    env.Alias("install", env.Install(os.path.join(install_dir, "include/medic"), h))

for k, contents in targets.items():
    for c in contents:
        for path in __getAllPaths(c):
            path_split = re.split(r"[\\/]", path)

            dirname = install_dir
            if "include" in path_split:
                dirname = os.path.join(install_dir, "include/medic")
            elif "lib" in path_split:
                dirname = os.path.join(install_dir, "lib")
            elif "bin" in path_split:
                dirname = os.path.join(install_dir, "bin")
            elif "Karte" in path_split:
                dirname = os.path.join(install_dir, "plugins/Karte")
            elif "Tester" in path_split:
                dirname = os.path.join(install_dir, "plugins/Tester")
            elif "py" in path_split:
                dirname = os.path.join(install_dir, "py")
            elif "python" == path_split[0]:
                dirname = os.path.join(install_dir, "py", *path_split[1:-1])
            else:
                print("UNKNOWN INSTALL TARGET : {}".format(path))
                continue

            env.Alias("install", env.Install(dirname, path))

env["ZIPROOT"] = os.path.join(install_dir, "..")

env.Alias("package", env.Zip(package_file, [install_dir, os.path.join(install_dir, "../medic.mod")]))
env.Default(["medicAll"])
