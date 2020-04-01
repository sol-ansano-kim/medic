from medic cimport *
from numbers import Number
from cython.operator cimport dereference
from cython.operator cimport preincrement
import fnmatch
import os
import re
import sys
import imp
import copy
import logging
from maya import OpenMaya


LogDebug = logging.DEBUG
LogWarning = logging.WARNING
LogError = logging.ERROR


class Statics:
    TesterPathEnvName = "MEDIC_TESTER_PATH"
    KartePathEnvName = "MEDIC_KARTE_PATH"
    RePluginExt = re.compile("[.]%s$" % ("so" if "linux" in sys.platform else "bundle" if "darwin" == sys.platform else "dll"), re.IGNORECASE)
    RePyPluginExt = re.compile("[.]py$", re.IGNORECASE)
    ReKarteExt = re.compile("[.]karte$", re.IGNORECASE)
    RePyExt = re.compile("[.]py$", re.IGNORECASE)
    SelectionList = OpenMaya.MSelectionList()

    Wait = 0
    Done = 1
    Failed = 2
    Suspended = 3

    Logger = logging.getLogger("Medic")
    __stream_handler = logging.StreamHandler()
    __stream_handler.setFormatter(logging.Formatter("[%(name)s] %(levelname)s : '%(message)s'"))

    Logger.addHandler(__stream_handler)
    Logger.setLevel(LogWarning)
    Logger.propagate = False

    @staticmethod
    def ImportModule(path):
        module = None

        try:
            module = imp.load_source(os.path.splitext(os.path.basename(path))[0], path)
        except Exception as e:
            Statics.Debug("Failed to load module '{}' : '{}'".format(path, e))

        return module

    @staticmethod
    def Debug(msg):
        Statics.Logger.debug(msg)

    @staticmethod
    def Warning(msg):
        Statics.Logger.warning(msg)

    @staticmethod
    def Error(msg):
        Statics.Logger.error(msg)


def SetLogLevel(log_level):
    Statics.Logger.setLevel(log_level)


class Types:
    Null = MdTypes.MdNull
    Bool = MdTypes.MdBool
    Int = MdTypes.MdInt
    Float = MdTypes.MdFloat
    String = MdTypes.MdString
    BoolArray = MdTypes.MdBoolArray
    IntArray = MdTypes.MdIntArray
    FloatArray = MdTypes.MdFloatArray
    StringArray = MdTypes.MdStringArray


class TesterScopes:
    NodeTester = MdTesterScope.MdNodeTester
    SceneTester = MdTesterScope.MdSceneTester
    AssetTester = MdTesterScope.MdAssetTester


cdef class Parameter:
    cdef MdParameter *ptr

    def __cinit__(self):
        pass

    @staticmethod
    def Create(name, label, typ, defaultValue):
        pram = Parameter()
        if typ == Types.Bool or typ == Types.BoolArray:
            Statics.Debug("Create a new bool Parameter")
            pram.ptr = MdParameter.Create[bint](name, label, typ, defaultValue, NULL)
        elif typ == Types.Int or typ == Types.IntArray:
            Statics.Debug("Create a new int Parameter")
            pram.ptr = MdParameter.Create[int](name, label, typ, defaultValue, NULL)
        elif typ == Types.Float or typ == Types.FloatArray:
            Statics.Debug("Create a new float Parameter")
            pram.ptr = MdParameter.Create[float](name, label, typ, defaultValue, NULL)
        elif typ == Types.String or typ == Types.StringArray:
            Statics.Debug("Create a new string Parameter")
            pram.ptr = MdParameter.Create[string](name, label, typ, defaultValue, NULL)
        else:
            Statics.Warning("Not supported type")

            return None

        return pram


cdef class ParamContainer:
    cdef bint needToDelete
    cdef MdParamContainer *ptr

    def __cinit__(self):
        needToDelete = False

    @staticmethod
    def Create():
        Statics.Debug("Create a new ParamContainer")
        con = ParamContainer()
        con.ptr = new MdParamContainer()
        needToDelete = True
        return con

    def append(self, Parameter param):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        return self.ptr.append(param.ptr)

    def set(self, paramName, value, index=0):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return False

        cdef MdParameter *ptr = self.ptr.getParam(<string>paramName)

        if ptr == NULL:
            return False

        typ = ptr.getType()

        if typ == Types.Bool or typ == Types.BoolArray:
            return self.ptr.set[bint](paramName, value, <size_t>index)
        elif typ == Types.Int or typ == Types.IntArray:
            return self.ptr.set[int](paramName, value, <size_t>index)
        elif typ == Types.Float or typ == Types.FloatArray:
            return self.ptr.set[float](paramName, value, <size_t>index)
        elif typ == Types.String or typ == Types.StringArray:
            return self.ptr.set[string](paramName, value, <size_t>index)

        return False

    def names(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return []

        return self.ptr.names()

    def get(self, paramName, index=0):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        cdef bint boolValue
        cdef int intValue
        cdef float floatValue
        cdef string stringValue

        cdef MdParameter *ptr = self.ptr.getParam(<string>paramName)

        if ptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        typ = ptr.getType()

        if typ == Types.Bool or typ == Types.BoolArray:
            self.ptr.get[bint](paramName, boolValue, <size_t>index)
            return boolValue
        elif typ == Types.Int or typ == Types.IntArray:
            self.ptr.get[int](paramName, intValue, <size_t>index)
            return intValue
        elif typ == Types.Float or typ == Types.FloatArray:
            self.ptr.get[float](paramName, floatValue, <size_t>index)
            return floatValue
        elif typ == Types.String or typ == Types.StringArray:
            self.ptr.get[string](paramName, stringValue, <size_t>index)
            return stringValue

        return None

    def getParamInfos(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return []

        return_list = []

        cdef std_vector[string] names = self.ptr.names()
        cdef MdParameter *param

        for n in names:
            param = self.ptr.getParam(n)
            return_list.append((n, param.getLabel(), param.getType(), self.getDefault(n)))

        return return_list

    def getDefault(self, paramName):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        cdef bint boolValue
        cdef int intValue
        cdef float floatValue
        cdef string stringValue

        cdef MdParameter *ptr = self.ptr.getParam(<string>paramName)

        if ptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        typ = ptr.getType()

        if typ == Types.Bool or typ == Types.BoolArray:
            self.ptr.getDefault[bint](paramName, boolValue)
            return boolValue
        elif typ == Types.Int or typ == Types.IntArray:
            self.ptr.getDefault[int](paramName, intValue)
            return intValue
        elif typ == Types.Float or typ == Types.FloatArray:
            self.ptr.getDefault[float](paramName, floatValue)
            return floatValue
        elif typ == Types.String or typ == Types.StringArray:
            self.ptr.getDefault[string](paramName, stringValue)
            return stringValue

        return None

    def __dealloc__(self):
        if self.needToDelete and self.ptr != NULL:
            del(self.ptr)


cdef class Node:
    cdef MdNode *ptr
    cdef bint needToDelete
    cdef object __obj
    cdef object __dg
    cdef object __dag

    def __cinit__(self):
        needToDelete = False

    def __dealloc__(self):
        if self.needToDelete:
            del(self.ptr)

    def initialize(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return False

        self.__obj = OpenMaya.MObject()
        self.__dg = OpenMaya.MFnDependencyNode()
        self.__dag = OpenMaya.MFnDagNode()
        Statics.SelectionList.clear()
        Statics.SelectionList.add(self.name())
        Statics.SelectionList.getDependNode(0, self.__obj)
        self.__dg.setObject(self.__obj)
        if self.isDag():
            self.__dag.setObject(self.__obj)

        return True

    @staticmethod
    def Create(name):
        Statics.Debug("Create a new Node")
        node = Node()
        node.ptr = new MdNode(<string>name)
        node.needToDelete = True
        node.initialize()
        return node

    def name(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return ""

        return self.ptr.name()

    def type(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return ""

        return self.ptr.type()

    def dag(self):
        if not self.isDag():
            return None

        return self.__dag

    def dg(self):
        return self.__dg

    def object(self):
        return self.__obj

    def isDag(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return False

        return self.ptr.isDag()

    def getPath(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return OpenMaya.MDagPath()

        if not self.isDag():
            return OpenMaya.MDagPath()

        path = OpenMaya.MDagPath()
        self.__dag.getPath(path)
        return path

    def parents(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return []

        if not self.isDag():
            return []

        parent_list = []

        cdef MdNodeContainer *parents = new MdNodeContainer()

        self.ptr.parents(parents)
        it = parents.iterator()

        cdef MdNode *n
        while (not it.isDone()):
            n = it.next()
            new_node = Node.Create(n.name())
            parent_list.append(new_node)

        del(parents)
        return parent_list

    def children(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return []

        if not self.isDag():
            return []

        child_list = []

        cdef MdNodeContainer *children = new MdNodeContainer()

        self.ptr.children(children)
        it = children.iterator()

        cdef MdNode *n
        while (not it.isDone()):
            n = it.next()
            new_node = Node.Create(n.name())
            child_list.append(new_node)

        del(children)
        return child_list


cdef class Context:
    cdef MdContext *ptr

    def __cinit__(self):
        pass

    @staticmethod
    def Create(name):
        Statics.Debug("Create a new Context")
        context = Context()
        context.ptr = new MdContext(<string>name)

        return context

    def name(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return ""

        return self.ptr.name()

    def params(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        cdef MdParamContainer* conptr = NULL

        conptr = self.ptr.params()
        if conptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        con = ParamContainer()
        con.ptr = conptr

        return con


cdef class Tester:
    cdef MdTester *ptr

    def __cinit__(self):
        pass

    def Scope(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        return self.ptr.Scope()

    def Name(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return ""

        return self.ptr.Name()

    def Match(self, Node node):
        return self.ptr.Match(node.ptr)

    def Match(self, Context context):
        return self.ptr.Match(context.ptr)

    def Description(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return ""

        return self.ptr.Description()

    def Dependencies(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return []

        return self.ptr.Dependencies()

    def IsFixable(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return False

        return self.ptr.IsFixable()

    def GetParameters(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        con = ParamContainer()
        con.ptr = self.ptr.GetParameters()
        return con

    def fix(self, Report report, ParamContainer params):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return False

        return self.ptr.fix(report.ptr, params.ptr)

    def initialize(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
        else:
            self.ptr.initialize()

    def test(self, Node node):
        r = self.ptr.test(node.ptr)
        if r == NULL:
            return None

        report = Report()
        report.ptr = r

        return report


cdef class Report:
    cdef MdReport *ptr

    def __cinit__(self):
        pass

    def addSelection(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return

        self.ptr.addSelection()

    def node(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        cdef MdNode *nodeptr = self.ptr.node()

        if nodeptr == NULL:
            return None

        new_node = Node()
        new_node.ptr = nodeptr
        new_node.initialize()

        return new_node

    def context(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        cdef MdContext *contextptr = self.ptr.context()

        if contextptr == NULL:
            return None

        context = Context()
        context.ptr = contextptr

        return context


cdef class Karte:
    cdef MdKarte *ptr
    cdef object py_testers

    def __cinit__(self):
        self.py_testers = []

    def Name(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return ""

        return self.ptr.Name()

    def Description(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return ""

        return self.ptr.Description()

    def Visible(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return False

        return self.ptr.Visible()

    def hasTester(self, tester):
        if isinstance(tester, PyTester):
            return self.hasPyTester(tester)

        else:
            return self.__hasTester(tester)

    def __hasTester(self, Tester tester):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return False

        return self.ptr.hasTester(tester.ptr)

    def hasPyTester(self, tester):
        return tester in self.py_testers

    def pyTesters(self):
        for t in self.py_testers:
            yield t

    def cppTesters(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return []

        testers = []
        cdef MdTesterIterator it = self.ptr.testers()
        cdef MdTester *t

        while (not it.isDone()):
            t = it.next()
            if t != NULL:
                tester = Tester()
                tester.ptr = t
                testers.append(tester)

        for tr in testers:
            yield tr

    def testers(self):
        testers = []
        for t in self.cppTesters():
            testers.append(t)

        for t in self.pyTesters():
            testers.append(t)

        return testers

    def addPyTester(self, tester):
        if tester in self.py_testers:
            return False

        self.py_testers.append(tester)
        return True


cdef class Visitor:
    cdef MdVisitor *ptr
    cdef object __report_cache
    cdef bint scene_collected
    cdef bint assets_collected

    def __cinit__(self):
        self.ptr = new MdVisitor()
        self.__report_cache = {}
        self.scene_collected = False
        self.assets_collected = False

    def __dealloc__(self):
        if self.ptr != NULL:
            del(self.ptr)

    def selectionOnly(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return False

        return self.ptr.selectionOnly()

    def setSelectionOnly(self, v):
        if self.ptr != NULL:
            self.ptr.setSelectionOnly(v)

    def collectScene(self):
        pass

    def collectAssets(self):
        pass

    def clearScene(self):
        if self.ptr != NULL:
            self.ptr.clearScene()

        self.scene_collected = False

    def clearAssets(self):
        if self.ptr != NULL:
            self.ptr.clearAssets()

        self.assets_collected = False

    def setScene(self, Context scene):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return False

        self.ptr.setScene(scene.ptr)
        self.scene_collected = True

        return True

    def scene(self):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return None

        cdef MdContext *scn_ptr = self.ptr.scene()

        if scn_ptr == NULL:
            return None

        con = Context()
        con.ptr = scn_ptr

        return con

    def addAsset(self, Context asset):
        if self.ptr == NULL:
            Statics.Warning("NULL pointer")
            return False

        self.assets_collected = True

        return self.ptr.addAsset(asset.ptr)

    def assets(self):
        cdef MdContextIterator it = self.ptr.assets()
        cdef MdContext *c
        assets = []

        while (not it.isDone()):
            n = it.next()
            if n != NULL:
                asset = Context()
                asset.ptr = n
                assets.append(asset)

        return assets

    def getOptions(self):
        cdef MdParamContainer* con = NULL
        cdef std_vector[string] names
        cdef MdParameter *param = NULL
        cdef bint boolValue
        cdef int intValue
        cdef float floatValue
        cdef string stringValue

        return_dict = {}

        option_keys = self.ptr.getOptionKeys()
        for k in option_keys:
            con = self.ptr.getOptions(<string>k)
            if con == NULL:
                continue

            cur_dict = {}

            names = con.names()
            for n in names:
                param = con.getParam(n)
                if (param == NULL):
                    continue

                typ = param.getType()

                if not param.isArray():
                    if typ == Types.Bool:
                        con.get[bint](n, boolValue)
                        cur_dict[n] = boolValue
                    elif typ == Types.Int:
                        con.get[int](n, intValue)
                        cur_dict[n] = intValue
                    elif typ == Types.Float:
                        con.get[float](n, floatValue)
                        cur_dict[n] = floatValue
                    elif typ == Types.String:
                        con.get[string](n, stringValue)
                        cur_dict[n] = stringValue
                else:
                    values = []
                    cur_dict[n] = values

                    if typ == Types.BoolArray:
                        for i in range(param.size()):
                            con.get[bint](n, boolValue, <size_t>i)
                            values.append(boolValue)

                    elif typ == Types.IntArray:
                        for i in range(param.size()):
                            con.get[int](n, intValue, <size_t>i)
                            values.append(intValue)

                    elif typ == Types.FloatArray:
                        for i in range(param.size()):
                            con.get[float](n, floatValue, <size_t>i)
                            values.append(floatValue)

                    elif typ == Types.StringArray:
                        for i in range(param.size()):
                            con.get[string](n, stringValue, <size_t>i)
                            values.append(stringValue)

            if cur_dict:
                return_dict[k] = cur_dict

        return return_dict

    def __getArrayType(self, values):
        list_dt = None

        for v in values:
            dt = type(v)

            if isinstance(v, basestring):
                dt = str

            if list_dt is None:
                list_dt = dt
                continue

            if list_dt == dt:
                continue

            if list_dt == float and issubclass(dt, Number):
                continue

            if list_dt == int and issubclass(dt, Number):
                if dt == float:
                    list_dt = float

                continue

            if list_dt == bool and issubclass(dt, Number):
                if dt != bool:
                    list_dt = dt

                continue

            return None

        return list_dt

    def setOptions(self, optionDict):
        cdef MdParamContainer* con = NULL

        self.ptr.clearOptions()

        if not isinstance(optionDict, dict):
            return False

        for key, options in optionDict.items():
            if not isinstance(key, basestring) or not isinstance(options, dict):
                Statics.Warning("Invalid Option '{}' - {}".format(key, options))
                continue

            con = self.ptr.getOptions(key)

            for n, v in options.items():
                if not isinstance(n, basestring):
                    Statics.Warning("Invalid Option '{}' - {}".format(n, v))

                if isinstance(v, list):
                    values = []
                    if len(v) == 0:
                        dt = float
                    else:
                        dt = self.__getArrayType(v)
                        if dt is None:
                            Statics.Warning("Invalid Option List'{}'".format(v))
                            continue

                        values = map(lambda x: dt(x), v)

                    if dt == bool:
                        parm = MdParameter.Create(n, n, Types.BoolArray, False, NULL)
                        parm.resize(len(values))
                        con.append(parm)
                        for i, v in enumerate(values):
                            con.set[bint](n, v, <size_t>i)

                    elif dt == int:
                        parm = MdParameter.Create(n, n, Types.IntArray, 0, NULL)
                        parm.resize(len(values))
                        con.append(parm)
                        for i, v in enumerate(values):
                            con.set[int](n, v, <size_t>i)

                    elif dt == float:
                        parm = MdParameter.Create(n, n, Types.FloatArray, 0.0, NULL)
                        parm.resize(len(values))
                        con.append(parm)
                        for i, v in enumerate(values):
                            con.set[float](n, v, <size_t>i)

                    elif dt == str:
                        parm = MdParameter.Create(n, n, Types.StringArray, "", NULL)
                        parm.resize(len(values))
                        con.append(parm)
                        for i, v in enumerate(values):
                            con.set[string](n, v, <size_t>i)

                    else:
                        Statics.Warning("Invalid value type {}:{} '{}'".format(key, n, dt))
                        continue

                else:
                    if isinstance(v, bool):
                        parm = MdParameter.Create(n, n, Types.Bool, False, NULL)
                        con.append(parm)
                        con.set[bint](n, bool(v))

                    elif isinstance(v, int):
                        parm = MdParameter.Create(n, n, Types.Int, 0, NULL)
                        con.append(parm)
                        con.set[int](n, int(v))

                    elif isinstance(v, float):
                        parm = MdParameter.Create(n, n, Types.Float, 0.0, NULL)
                        con.append(parm)
                        con.set[float](n, float(v))

                    elif isinstance(v, str):
                        parm = MdParameter.Create(n, n, Types.String, "", NULL)
                        con.append(parm)
                        con.set[string](n, str(v))

                    else:
                        Statics.Warning("Invalid value type {}:{} '{}'".format(key, n, type(v)))
                        continue

        return True

    def initializeTester(self, tester):
        if isinstance(tester, PyTester):
            tester.initialize()
            options = self.getOptions()
            tester.setOptions(options.get(tester.Name(), {}))
        else:
            self._initializeTester(tester)

    def _initializeTester(self, Tester tester):
        self.ptr.initializeTester(tester.ptr)

    def finalizeTester(self, tester):
        if isinstance(tester, PyTester):
            tester.finalize()
        else:
            self._finalizeTester(tester)

    def _finalizeTester(self, Tester tester):
        self.ptr.finalizeTester(tester.ptr)

    def addReport(self, tester, report):
        if tester not in self.__report_cache:
            self.__report_cache[tester] = []
        self.__report_cache[tester].append(report)

    def canVisit(self, karte=None, tester=None, node=None, context=None):
        return True

    def visitKarte(self, karte):
        if not self.canVisit(karte, None, None, None):
            for name, tester_dict in testers.items():
                tester_dict["state"] = Statics.Done
            return

        self.__report_cache = {}

        over_states = [Statics.Done, Statics.Failed, Statics.Suspended]

        testers = {}

        for t in karte.testers():
            testers[t.Name()] = {"tester": t, "dep": t.Dependencies(), "state": Statics.Wait}

        pynodes = self.__nodes()
        while (True):
            for name, tester_dict in testers.items():
                if tester_dict["state"] in over_states:
                    continue

                dependencies = tester_dict["dep"]

                need_to_wait = False
                available = True
                for dep in dependencies:
                    dep_dict = testers.get(dep)
                    if not dep_dict:
                        continue

                    st = dep_dict["state"]
                    if st == Statics.Failed or st == Statics.Suspended:
                        available = False
                        break

                    if st == Statics.Wait:
                        need_to_wait = True
                        break

                if not available:
                    tester_dict["state"] = Statics.Suspended
                    continue

                if need_to_wait:
                    continue

                tester = tester_dict["tester"]

                if not self.canVisit(karte, tester, None, None):
                    self.visitTester(tester)

                    if self.hasError(tester):
                        tester_dict["state"] = Statics.Failed
                    else:
                        tester_dict["state"] = Statics.Done
                else:
                    tester_dict["state"] = Statics.Done

            is_done = True
            for tester_dict in testers.values():
                st = tester_dict["state"]
                if st not in over_states:
                    is_done = False
                    break

            if is_done:
                break

    def visitTester(self, tester):
        if not self.canVisit(None, tester, None, None):
            return

        Statics.Debug("Start Tester '{}'".format(tester.Name()))

        if tester.Scope() == MdAssetTester and not self.assets_collected:
            self.collectAssets()
            self.assets_collected = True

        if tester.Scope() == MdSceneTester and not self.scene_collected:
            self.collectScene()
            self.scene_collected = True

        self.initializeTester(tester)

        self.__report_cache.pop(tester, [])

        if tester.Scope() == MdNodeTester:
            nodes = self.__nodes()
            for n in nodes:
                self.visitNode(tester, n)

        elif tester.Scope() == MdSceneTester:
            self.visitContext(tester, self.scene())

        elif tester.Scope() == MdAssetTester:
            for asset in self.assets():
                self.visitContext(tester, asset)

        self.finalizeTester(tester)

    def visitNode(self, tester, node):
        if tester.Match(node) and self.canVisit(None, tester, node, None):
            r = tester.test(node)
            if r is not None:
                self.addReport(tester, r)

    def visitContext(self, tester, context):
        if tester.Match(context) and self.canVisit(None, tester, None, context):
            r = tester.test(context)
            if r is not None:
                self.addReport(tester, r)

    def test(self, karte, tester):
        if not karte.hasTester(tester):
            return

        self.visitTester(tester)

    def testAll(self, karte):
        self.visitKarte(karte)

    def hasError(self, tester):
        if isinstance(tester, PyTester):
            return True if self.__report_cache.get(tester) else False

        return self.__hasError(tester)

    def __hasError(self, Tester tester):
        return self.ptr.hasError(tester.ptr)

    def reportAll(self):
        results = {}
        for t, r in self.__report_cache.items():
            results[t] = r[:]

        return results

    def report(self, tester):
        return self.__report_cache.get(tester, [])[:]

    def reset(self):
        self.ptr.reset()
        self.__report_cache = {}
        self.clearScene()
        self.clearAssets()

    def __nodes(self):
        cdef MdNodeIterator it = self.ptr.nodes()
        cdef MdNode *n
        nodes = []

        while (not it.isDone()):
            n = it.next()
            if n != NULL:
                new_node = Node()
                new_node.ptr = n
                new_node.initialize()
                nodes.append(new_node)

        return nodes


class PyKarteManager(object):
    __Instance = None
    __Kartes = {}

    def karteNames(self):
        return self.__Kartes.keys()

    def karte(self, name):
        return self.__Kartes.get(name, None)

    def regist(self, karte):
        if self.__Kartes.has_key(karte.Name()):
            return False

        self.__Kartes[karte.Name()] = karte
        return True

    def unload(self):
        self.__Kartes = {}

    def __new__(cls):
        if not cls.__Instance:
            cls.__Instance = super(PyKarteManager, cls).__new__(cls)

        return cls.__Instance

class PyTesterManager(object):
    __Instance = None
    __Testers = {}

    def regist(self, tester):
        if self.__Testers.has_key(tester.Name()):
            return False

        self.__Testers[tester.Name()] = tester
        return True

    def testerNames(self):
        return self.__Testers.keys()

    def tester(self, name):
        return self.__Testers.get(name, None)

    def unload(self):
        self.__Testers = {}

    def __new__(cls, existsList=[]):
        if not cls.__Instance:
            cls.__Instance = super(PyTesterManager, cls).__new__(cls)

        return cls.__Instance


cdef class __PluginManager:
    cdef MdPlugInManager *ptr
    cdef object __py_tester_manager
    cdef object __py_karte_manager

    def __cinit__(self):
        self.ptr = MdPlugInManager.Instance()

    def __init__(self):
        self.__py_tester_manager = PyTesterManager()
        self.__py_karte_manager = PyKarteManager()
        self.__registTesters()
        self.__registKartes()

    def __registTesters(self):
        search_dirs = map(lambda x : os.path.abspath(x), filter(lambda y : y, os.environ.get(Statics.TesterPathEnvName, "").split(os.pathsep)))
        for sdir in search_dirs:
            if not os.path.isdir(sdir):
                continue

            for f in os.listdir(sdir):
                if not Statics.RePluginExt.search(f):
                    continue

                full_path = os.path.join(sdir, f).replace("\\", "/")
                add_res = self.__addTester(full_path)
                if add_res == MdLoadingFailure:
                    Statics.Debug("Failed to load plugin : '{}'".format(full_path))
                elif add_res == MdExistsPlugin:
                    Statics.Debug("Tester exists already that has a same name : '{}'".format(full_path))
                else:
                    Statics.Debug("Tester Added : '{}'".format(full_path))

        exists_list = self.__cTesterNames()

        for sdir in search_dirs:
            if not os.path.isdir(sdir):
                continue

            for f in os.listdir(sdir):
                if not Statics.RePyPluginExt.search(f):
                    continue

                full_path = os.path.join(sdir, f).replace("\\", "/")

                module = Statics.ImportModule(full_path)

                if not hasattr(module, "Create"):
                    Statics.Debug("Failed to load plugin - No 'Create' function '{}'".format(full_path))
                    continue

                tester = module.Create()

                if not isinstance(tester, PyTester):
                    Statics.Debug("Failed to load plugin - It is not a instance of medic.PyTester '{}'".format(full_path))
                    continue

                if tester.Name() in exists_list:
                    Statics.Debug("Same tester exists already : '{}'".format(tester.Name()))
                    continue

                Statics.Debug("Tester Added : '{}'".format(full_path))
                self.__py_tester_manager.regist(tester)

    def __addTester(self, path):
        return self.ptr.addTester(<string>path)

    def __registKartes(self):
        ctesters = self.__cTesterNames()

        search_dirs = map(lambda x : os.path.abspath(x), filter(lambda y : y, os.environ.get(Statics.KartePathEnvName, "").split(os.pathsep)))
        for sdir in search_dirs:
            if not os.path.isdir(sdir):
                continue

            for f in os.listdir(sdir):
                if not Statics.ReKarteExt.search(f):
                    continue

                full_path = os.path.join(sdir, f).replace("\\", "/")

                krate_data = {}

                try:
                    with open(full_path, "r") as f:
                        karte_data = eval(f.read())

                except Exception as e:
                    Statics.Debug("Failed to load karte '{}' : '{}'".format(full_path, e))
                    continue

                name = karte_data.get("Name", "UNKNOWN")
                description = karte_data.get("Description", "")
                tester_patterns = karte_data.get("Testers", [])
                visible = karte_data.get("Visible", True)
                karte_testers = []

                for tester in ctesters:
                    for pt in tester_patterns:
                        if fnmatch.fnmatchcase(tester, pt):
                            karte_testers.append(tester)
                            break
                add_res = self.__addKarte(full_path, name, description, visible, karte_testers)
                if add_res == MdLoadingFailure:
                    Statics.Debug("Failed to load karte : '{}'".format(full_path))
                    continue

                elif add_res == MdExistsPlugin:
                    Statics.Debug("Same karte exists already : '{}'".format(full_path))
                    continue

                karte = self.karte(name)
                for pyt in self.__py_tester_manager.testerNames():
                    for pt in tester_patterns:
                        if fnmatch.fnmatchcase(pyt, pt):
                            karte.addPyTester(self.__py_tester_manager.tester(pyt))

                Statics.Debug("Karte Added : '{}'".format(full_path))

                self.__py_karte_manager.regist(karte)

    cdef __addKarte(self, filepath, name, description, visible, testers):
        cdef std_vector[string] tester_names;
        for t in testers:
            tester_names.push_back(<string>t)
        result = self.ptr.addKarte(<string>name, <string>description, <bint>visible, tester_names)

        if result:
            new_karte = Karte()
            new_karte.ptr = self.ptr.karte(<string>name)
            return self.__py_karte_manager.regist(new_karte)

        return False

    def tester(self, name):
        tester = self.__py_tester_manager.tester(name)
        if tester:
            return tester

        cdef MdTester *t

        t = self.ptr.tester(<string>name)
        if t != NULL:
            new_tester = Tester()
            new_tester.ptr = t
            return new_tester

        return None

    def karte(self, name):
        return self.__py_karte_manager.karte(name)

    def __cTesterNames(self):
        name_list = []

        cdef std_vector[string] names = self.ptr.testerNames()
        cdef std_vector[string].iterator it = names.begin()

        while (it != names.end()):
            name_list.append(dereference(it))
            preincrement(it)

        return name_list

    def testerNames(self):
        return self.__cTesterNames() + self.__py_tester_manager.testerNames()

    def karteNames(self):
        return self.__py_karte_manager.karteNames()

    def reloadPlugins(self):
        Statics.Debug("Reload Plugins")

        self.__py_tester_manager.unload()
        self.__py_karte_manager.unload()
        self.ptr.unload()
        self.__registTesters()
        self.__registKartes()


class PluginManager(object):
    __Instance = None

    def __new__(cls):
        if not cls.__Instance:
            cls.__Instance = super(PluginManager, cls).__new__(cls)
            cls.__PluginManager = __PluginManager()
            cls.tester = cls.__PluginManager.tester
            cls.karte = cls.__PluginManager.karte
            cls.testerNames = cls.__PluginManager.testerNames
            cls.karteNames = cls.__PluginManager.karteNames
            cls.reloadPlugins = cls.__PluginManager.reloadPlugins

        return cls.__Instance


class PyReport(object):
    def __init__(self, node_or_context, components=None):
        Statics.Debug("Create a new PyReport")
        super(PyReport, self).__init__()
        self.__node = None
        self.__context = None

        if isinstance(node_or_context, Node):
            self.__node = node_or_context
        if isinstance(node_or_context, Context):
            self.__context = node_or_context

        self.__components = components
        self.__has_components = False if components is None else True

    def addSelection(self):
        if self.__node is None:
            return

        Statics.SelectionList.clear()

        if self.__node.isDag():
            if self.__has_components:
                Statics.SelectionList.add(self.__node.getPath(), self.__components)
            else:
                Statics.SelectionList.add(self.__node.getPath())
        else:
            Statics.SelectionList.add(self.__node.object())

        OpenMaya.MGlobal.setActiveSelectionList(Statics.SelectionList, OpenMaya.MGlobal.kAddToList)

    def node(self):
        return self.__node

    def context(self):
        return self.__context


class PyTester(object):
    def __init__(self):
        Statics.Debug("Create a new PyTester")
        super(PyTester, self).__init__()
        self.__options = {}

    def setOptions(self, options):
        self.__options = options

    def getOptions(self):
        return copy.deepcopy(self.__options)

    def initialize(self):
        pass

    def finalize(self):
        pass

    def Scope(self):
        return MdNodeTester

    def Name(self):
        return "TesterBase"

    def Description(self):
        return ""

    def Dependencies(self):
        return []

    def Match(self, node_or_context):
        return False

    def IsFixable(self):
        return False

    def GetParameters(self):
        return ParamContainer.Create()

    def test(self, node_or_context):
        return None

    def fix(self, report, params):
        return False
