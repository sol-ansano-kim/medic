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


mayaver = excons.GetArgument("mayaver", None)
if not mayaver:
    mayaver = excons.GetArgument("with-maya", None)


defs = []
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
             "type": "sharedlib",
             "alias": "medic-lib",
             "defs": defs,
             "cppflags": cppflags,
             "incdirs": ["include"],
             "srcs": excons.glob("src/medic/*.cpp"),
             "symvis": "default",
             "install": {out_incdir + "/medic": excons.glob("include/medic/*.h")},
             "custom": [python.SoftRequire]})


prjs.append({"name": "_medic",
             "type": "dynamicmodule",
             "alias": "medic-py",
             "ext": python.ModuleExtension(),
             "prefix": "py",
             "rpath": out_libdir,
             "cppflags": cppflags,
             "incdirs": ["pybind11/include", "include"],
             "srcs": excons.glob("src/medic/py/*.cpp"),
             "libdirs": [out_libdir],
             "libs": ["medic"],
             "custom": [python.SoftRequire]})


if mayaver:
    ## build maya
    prjs.append({"name": "mayaMedic",
                 "type": "sharedlib",
                 "alias": "medic-maya",
                 "defs": defs,
                 "cppflags": cppflags,
                 "incdirs": ["include"],
                 "srcs": excons.glob("src/medic/maya/*.cpp"),
                 "symvis": "default",
                 "install": {out_incdir + "/medic/maya": excons.glob("include/medic/maya/*.h")},
                 "custom": [python.SoftRequire, maya.Require]})


for test in excons.glob("test/*.cpp"):
    test_base = os.path.splitext(os.path.basename(test))[0]

    prjs.append({"name": test_base,
                 "type": "testprograms",
                 "defs": defs,
                 "cppflags": cppflags,
                 "incdirs": ["include"],
                 "prefix": "test",
                 "srcs": [test],
                 "libdirs": [out_libdir],
                 "libs": ["medic"],
                 "rpath": out_libdir,
                 "custom": [python.Require]})

for test_plugin in excons.glob("test/testers/*.cpp"):
    plug_base = os.path.splitext(os.path.basename(test_plugin))[0]

    prjs.append({"name": plug_base,
                 "type": "dynamicmodule",
                 "defs": defs,
                 "cppflags": cppflags,
                 "incdirs": ["include"],
                 "srcs": [test_plugin],
                 "libs": ["medic"],
                 "libdirs": [out_libdir],
                 "prefix": "test/testers",
                 "symvis": "default",
                 "rpath": out_libdir,
                 "custom": [python.SoftRequire]})


excons.DeclareTargets(env, prjs)
