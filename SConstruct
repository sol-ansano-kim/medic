import excons
from excons.tools import python
from excons.tools import maya
import os
import sys
import shutil
import re


major = 2
minor = 0
patch = 0


maya.SetupMscver()
env = excons.MakeBaseEnv()


medic_static = (excons.GetArgument("medic-static", 0, int) != 0)

mayaver = excons.GetArgument("mayaver", None)
if not mayaver:
    mayaver = excons.GetArgument("with-maya", None)


defs = []
if medic_static:
    defs.append("MEDIC_DLL")

if sys.platform == "win32":
    cppflags = " /wd4100 /wd4505 /wd4701 /wd4127 /wd4189 /wd4005 /wd4510 /wd4512 /wd4610 /wd4211 /wd4702 /wd4706 /wd4310"
else:
    cppflags = " -Wno-unused-parameter -std=c++11"
prjs = []
customs = []


out_incdir = excons.OutputBaseDirectory() + "/include"
out_libdir = excons.OutputBaseDirectory() + "/lib/"
out_pydir = excons.OutputBaseDirectory() + "/py/"


## build
prjs.append({"name": "medic",
             "type": ("staticlib" if medic_static else "sharedlib"),
             "alias": "medic-lib",
             "defs": defs,
             "cppflags": cppflags,
             "incdirs": ["include"],
             "srcs": excons.glob("src/medic/*.cpp"),
             "symvis": "default",
             "install": {out_incdir + "/medic": excons.glob("include/medic/*.h")},
             "custom": [python.SoftRequire]})


def MedicName():
    name = "medic"
    if sys.platform == "win32" and medic_static:
        name = "lib" + name

    return name


def MedicPath():
    name = MedicName()

    if sys.platform == "win32":
        libname = name + ".lib"
    else:
        libname = "lib" + name + (".a" if medic_static else excons.SharedLibraryLinkExt())

    return out_libdir + "/" + libname


def RequireMedic(env):
    if not medic_static:
        env.Append(CPPDEFINES=["MEDIC_DLL"])

    env.Append(CPPPATH=[out_incdir])
    env.Append(LIBPATH=[out_libdir])
    excons.Link(env, MedicPath(), static=medic_static, force=True, silent=True)



prjs.append({"name": "_medic",
             "type": "dynamicmodule",
             "alias": "medic-py",
             "ext": python.ModuleExtension(),
             "prefix": "py",
             "rpath": out_libdir,
             "cppflags": cppflags,
             "incdirs": ["pybind11/include", "include"],
             "srcs": excons.glob("src/medicBinding/*.cpp"),
             "deps": ["medic"],
             "custom": [python.SoftRequire, RequireMedic]})


if mayaver:
    ## build maya
    prjs.append({"name": "mayaMedic",
                 "type": "sharedlib",
                 "alias": "medic-maya",
                 "defs": defs,
                 "cppflags": cppflags,
                 "incdirs": ["include"],
                 "srcs": excons.glob("src/medicMaya/*.cpp"),
                 "symvis": "default",
                 "deps": ["medic"],
                 "install": {out_incdir + "/medicMaya": excons.glob("include/medicMaya/*.h")},
                 "custom": [python.SoftRequire, maya.Require, RequireMedic]})

    prjs.append({"name": "_medicMaya",
                 "type": "dynamicmodule",
                 "alias": "medic-maya-py",
                 "ext": python.ModuleExtension(),
                 "prefix": "py",
                 "rpath": out_libdir,
                 "cppflags": cppflags,
                 "incdirs": ["pybind11/include", "include"],
                 "srcs": excons.glob("src/medicMayaBinding/*.cpp"),
                 "deps": ["medic"],
                 "libdirs": [out_libdir],
                 "libs": ["mayaMedic"],
                 "custom": [python.SoftRequire, maya.Require, RequireMedic]})


for test in excons.glob("test/*.cpp"):
    test_base = os.path.splitext(os.path.basename(test))[0]

    prjs.append({"name": test_base,
                 "type": "testprograms",
                 "defs": defs,
                 "cppflags": cppflags,
                 "prefix": "test",
                 "srcs": [test],
                 "rpath": out_libdir,
                 "deps": ["medic"],
                 "custom": [python.Require, RequireMedic]})

for test_plugin in excons.glob("test/testers/*.cpp"):
    plug_base = os.path.splitext(os.path.basename(test_plugin))[0]

    prjs.append({"name": plug_base,
                 "type": "dynamicmodule",
                 "defs": defs,
                 "cppflags": cppflags,
                 "srcs": [test_plugin],
                 "prefix": "test/testers",
                 "symvis": "default",
                 "deps": ["medic"],
                 "rpath": out_libdir,
                 "custom": [python.SoftRequire, RequireMedic]})


excons.DeclareTargets(env, prjs)

Export("MedicName MedicPath RequireMedic")
