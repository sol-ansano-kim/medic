from _medic import *
from maya import OpenMaya


def Tester(name):
   return PluginManager().tester(name)


def Karte(name):
   return PluginManager().karte(name)


def TesterNames():
   return PluginManager().testerNames()


def KarteNames():
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

   elif isinstance(_input, basestring):
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
