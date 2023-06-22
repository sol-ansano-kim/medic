import os

if hasattr(os, "add_dll_directory"):
   lib_path = os.environ.get("MEDIC_LIB_PATH")
   if not lib_path:
      lib_path = os.path.join(os.path.dirname(os.path.dirname(__file__)), "lib")

   if lib_path and os.path.isdir(lib_path):
      os.add_dll_directory(lib_path)

from _medic import *
from maya import OpenMaya


def GetTester(name):
   return PluginManager().tester(name)


def GetKarte(name):
   return PluginManager().karte(name)


def GetTesterNames():
   return PluginManager().testerNames()


def GetKarteNames():
   return PluginManager().karteNames()


def ReloadPlugins():
   PluginManager().reloadPlugins()


def IsTemplated(_input):
   if not _input:
      return False

   dp = None

   if isinstance(_input, OpenMaya.MDagPath):
      dp = OpenMaya.MDagPath(_input)

   elif isinstance(_input, OpenMaya.MObject):
      try:
         dn = OpenMaya.MFnDagNode(_input)
         dp = OpenMaya.MDagPath()
         dn.getPath(dp)
      except:
         pass

   elif isinstance(_input, Node):
      if _input.isDag():
         dp = _input.getPath()

   elif isinstance(_input, str):
      sl = OpenMaya.MSelectionList()
      try:
         _dp = OpenMaya.MDagPath()
         sl.add(_input)
         sl.getDagPath(0, _dp)
         dp = _dp
      except:
         pass

   if dp:
      overridden = False
      while dp.length() > 0:
         dn = OpenMaya.MFnDagNode(dp)
         if not overridden:
            try:
               plug = dn.findPlug("overrideEnabled");
               if plug.asBool():
                  overridden = True
                  plug = dn.findPlug("overrideDisplayType")
                  if plug.asShort() == 1:
                     return True
            except:
               pass
         try:
            plug = dn.findPlug("template")
            if plug.asBool():
               return True
         except:
            pass
         try:
            dp.pop()
         except:
            break

   return False
