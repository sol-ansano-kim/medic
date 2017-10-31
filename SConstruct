import excons
from excons.tools import python
import os
import sys
import shutil
import re


major = 2
minor = 0
patch = 0


env = excons.MakeBaseEnv()


defs = []
if sys.platform == "win32":
    cppflags = " /wd4100 /wd4505 /wd4701 /wd4127 /wd4189 /wd4005 /wd4510 /wd4512 /wd4610 /wd4211 /wd4702 /wd4706 /wd4310"
else:
    cppflags = " -Wno-unused-parameter -Wunused-variable"
prjs = []
customs = []


out_incdir = excons.OutputBaseDirectory() + "/include"
out_libdir = excons.OutputBaseDirectory() + "/lib/"
out_pydir = excons.OutputBaseDirectory() + "/py/"


## build
headers = excons.glob("include/medic/*")


prjs.append({"name": "medic",
             "type": "sharedlib",
             "alias": "medic-lib",
             "defs": defs,
             "cppflags": cppflags,
             "incdirs": [out_incdir],
             "srcs": excons.glob("src/medic/*.cpp"),
             "symvis": "hidden",
             "install": {out_incdir + "/medic": headers},
             "custom": customs})


excons.DeclareTargets(env, prjs)
