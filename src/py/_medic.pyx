from medic cimport *
from cython.operator cimport dereference
from cython.operator cimport preincrement
import fnmatch
import os
import re
import sys
from maya import OpenMaya


class StaticVars:
    TesterPathEnvName = "MEDIC_TESTER_PATH"
    KartePathEnvName = "MEDIC_KARTE_PATH"
    RePluginExt = re.compile("[.]%s$" % ("so" if "linux" in sys.platform else "bundle" if "darwin" == sys.platform else "dll"), re.IGNORECASE)
    ReKarteExt = re.compile("[.]karte$", re.IGNORECASE)
    RePyExt = re.compile("[.]py$", re.IGNORECASE)
    SelectionList = OpenMaya.MSelectionList()


cdef class Node:
    cdef MdNode *ptr
    cdef bint needToDelete
    m_obj = OpenMaya.MObject()
    m_dg = OpenMaya.MFnDependencyNode()
    m_dag = OpenMaya.MFnDagNode()

    def __cinit__(self):
        needToDelete = False

    def __dealloc__(self):
        if self.needToDelete:
            del(self.ptr)

    def initialize(self):
        if self.ptr == NULL:
            return False

        StaticVars.SelectionList.clear()
        StaticVars.SelectionList.add(self.name())
        StaticVars.SelectionList.getDependNode(0, self.m_obj)
        self.m_dg.setObject(self.m_obj)
        if self.isDag():
            self.m_dag.setObject(self.m_obj)

        return True

    @staticmethod
    def Create(name):
        node = Node()
        node.ptr = new MdNode(<string>name)
        node.needToDelete = True
        node.initialize()
        return node

    def name(self):
        if self.ptr == NULL:
            return ""

        return self.ptr.name()

    def type(self):
        if self.ptr == NULL:
            return ""

        return self.ptr.type()

    def dag(self):
        if not self.isDag():
            return None

        return self.m_dag

    def dg(self):
        return self.m_dg

    def object(self):
        return self.m_obj

    def isDag(self):
        if self.ptr == NULL:
            return False

        return self.ptr.isDag()

    def parents(self):
        if self.ptr == NULL:
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
            new_node = Node.CreateByName(n.name())
            parent_list.append(new_node)

        del(parents)
        return parent_list

    def children(self):
        if self.ptr == NULL:
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
            new_node = Node.CreateByName(n.name())
            child_list.append(new_node)

        del(children)
        return child_list


cdef class Tester:
    cdef MdTester *ptr

    def __cinit__(self):
        pass

    def name(self):
        if self.ptr == NULL:
            return ""

        return self.ptr.Name()

    def description(self):
        if self.ptr == NULL:
            return ""

        return self.ptr.Description()


cdef class Report:
    cdef MdReport *ptr

    def __cinit__(self):
        pass

    def node(self):
        if self.ptr == NULL:
            return None

        new_node = Node()
        new_node.ptr = self.ptr.node()
        return new_node

    def components(self):
        if self.ptr == NULL:
            return None

        if not self.ptr.hasComponents():
            return None

        ## TODO
        return None


cdef class Karte:
    cdef MdKarte *ptr

    def __cinit__(self):
        pass

    def name(self):
        if self.ptr == NULL:
            return ""

        return self.ptr.Name()

    def description(self):
        if self.ptr == NULL:
            return ""

        return self.ptr.Description()


cdef class Visitor:
    cdef MdVisitor *ptr

    def __cinit__(self):
        self.ptr = new MdVisitor()

    def __dealloc__(self):
        del(self.ptr)

    def visit(self, karte):
        self.__visit(karte)

    cdef __visit(self, Karte karte):
        self.ptr.visit(karte.ptr)

    cpdef results(self):
        cdef std_vector[MdTester *] testers = self.ptr.reportTesters()
        cdef std_vector[MdTester *].iterator it = testers.begin()
        cdef MdTester *tester
        cdef MdReportIterator reports
        cdef MdReport *report
        results = {}

        while (it != testers.end()):
            tester = dereference(it)
            new_tester = Tester()
            new_tester.ptr = tester
            reports = self.ptr.report(tester)
            results[new_tester] = []

            while (not reports.isDone()):
                report = reports.next()
                new_report = Report()
                new_report.ptr = report
                results[new_tester].append(new_report)

            preincrement(it)

        return results


cdef class PluginManager:
    cdef MdPlugInManager *ptr

    def __cinit__(self):
        self.ptr = MdPlugInManager.Instance()
        self.__registTesters()
        self.__registKartes()

    def __registTesters(self):
        search_dirs = map(lambda x : os.path.abspath(x), filter(lambda y : y, os.environ.get(StaticVars.TesterPathEnvName, "").split(os.pathsep)))
        for sdir in search_dirs:
            if not os.path.isdir(sdir):
                continue

            for f in os.listdir(sdir):
                if not StaticVars.RePluginExt.search(f):
                    continue

                full_path = os.path.join(sdir, f).replace("\\", "/")
                if self.__addTester(full_path) == MdLoadingFailure:
                    print "Load PlugIn Failed : %s" % full_path

    def __addTester(self, path):
        return self.ptr.addTester(<string>path)

    def __registKartes(self):
        testers = self.testerNames()

        search_dirs = map(lambda x : os.path.abspath(x), filter(lambda y : y, os.environ.get(StaticVars.KartePathEnvName, "").split(os.pathsep)))
        for sdir in search_dirs:
            if not os.path.isdir(sdir):
                continue

            for f in os.listdir(sdir):
                if not StaticVars.ReKarteExt.search(f):
                    continue

                full_path = os.path.join(sdir, f).replace("\\", "/")

                krate_data = {}

                try:
                    with open(full_path, "r") as f:
                        karte_data = eval(f.read())

                except:
                    print "Load Karte Failed : %s" % full_path
                    continue

                name = karte_data.get("Name", "UNKNOWN")
                description = karte_data.get("Description", "")
                tester_patterns = karte_data.get("Testers", [])
                karte_testers = []

                for tester in testers:
                    for pt in tester_patterns:
                        if fnmatch.fnmatchcase(tester, pt):
                            karte_testers.append(tester)
                            break

                if self.__addKarte(full_path, name, description, karte_testers) == MdLoadingFailure:
                    print "Load Karte Failed : %s" % full_path

    cdef __addKarte(self, filepath, name, description, testers):
        cdef std_vector[string] tester_names;
        for t in testers:
            tester_names.push_back(<string>t)
        return self.ptr.addKarte(<string>name, <string>description, tester_names)

    def tester(self, name):
        cdef MdTester *t

        t = self.ptr.tester(<string>name)
        if t == NULL:
            return None

        new_tester = Tester()
        new_tester.ptr = t

        return new_tester

    def karte(self, name):
        cdef MdKarte *k

        k = self.ptr.karte(<string>name)
        if k == NULL:
            return None

        new_karte = Karte()
        new_karte.ptr = k
        
        return new_karte

    def testerNames(self):
        name_list = []

        cdef std_vector[string] names = self.ptr.testerNames()
        cdef std_vector[string].iterator it = names.begin()

        while (it != names.end()):
            name_list.append(dereference(it))
            preincrement(it)

        return name_list

    def karteNames(self):
        name_list = []

        cdef std_vector[string] names = self.ptr.karteNames()
        cdef std_vector[string].iterator it = names.begin()

        while (it != names.end()):
            name_list.append(dereference(it))
            preincrement(it)

        return name_list
