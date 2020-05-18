from libcpp.string cimport string
from libcpp.vector cimport vector as std_vector
from libcpp.map cimport map as std_map


cdef extern from "maya/MObject.h" namespace "MEDIC":
    cdef cppclass MObject


cdef extern from "medic/action.h" namespace "MEDIC":
    cdef cppclass MdAction


cdef extern from "medic/parameter.h" namespace "MEDIC":
    cdef enum MdTypes:
        MdNull = 0
        MdBool
        MdInt
        MdFloat
        MdString
        MdBoolArray
        MdIntArray
        MdFloatArray
        MdStringArray

    cdef cppclass MdParameter:
        MdParameter()
        @staticmethod
        MdParameter *Create[T](string name, string label, MdTypes typ, const T &defaultValue, MdAction *action)
        MdTypes getType() const
        string getLabel() const
        bint getDefault[T](T &t) const
        bint resize(size_t s)
        size_t size() const
        bint isArray() const

    cdef cppclass MdParamContainer:
        MdParamContainer()
        bint append(MdParameter *p)
        MdParameter *getParam(string name)
        bint set[T](string paramName, const T &v, size_t index)
        bint get[T](string paramName, T &v, size_t index)
        bint getDefault[T](string paramName, T &v)
        std_vector[string] names()


cdef extern from "medic/node.h" namespace "MEDIC":
    cdef cppclass MdNodeIterator:
        MdNodeIterator()
        MdNodeIterator(MdNodeContainer *container)
        MdNode *next()
        bint isDone()

    cdef cppclass MdNodeContainer:
        MdNodeIterator iterator()

    cdef cppclass MdNode:
        MdNode(string name)
        string name() const
        string type() const
        bint isDag() const
        bint isAlive() const
        void parents(MdNodeContainer *container) const
        void children(MdNodeContainer *container) const


cdef extern from "medic/context.h" namespace "MEDIC":
    cdef cppclass MdContextIterator:
        MdContextIterator()
        MdContextIterator(MdContextContainer *container)
        MdContext *next()
        bint isDone()

    cdef cppclass MdContextContainer:
        MdContextIterator iterator()

    cdef cppclass MdContext:
        MdContext(string name)
        MdContext(string name, MdParamContainer* params)
        string name() const
        MdParamContainer* params()


cdef extern from "medic/report.h" namespace "MEDIC":
    cdef cppclass MdReport:
        MdNode *node()
        MdContext *context()
        void addSelection() const
        bint isAlive() const

    cdef cppclass MdReportIterator:
        MdReportIterator()
        MdReport *next()
        bint isDone()


cdef extern from "medic/tester.h" namespace "MEDIC":
    cdef enum MdTesterScope:
        MdNodeTester = 0
        MdAssetTester
        MdSceneTester

    cdef cppclass MdTesterIterator:
        MdTesterIterator();
        MdTester *next()
        bint isDone()

    cdef cppclass MdTester:
        MdTesterScope Scope()
        string Name()
        string Description()
        void initialize()
        void finalize()
        std_vector[string] Dependencies()
        bint Match(MdNode *node)
        bint Match(MdContext *context)
        MdReport *test(MdNode *node)
        MdReport *test(MdContext *node)
        bint IsFixable()
        MdParamContainer *GetParameters()
        bint fix(MdReport *report, MdParamContainer *params)


cdef extern from "medic/karte.h" namespace "MEDIC":
    cdef cppclass MdKarte:
        string Name() const
        string Description() const
        bint Visible() const
        bint hasTester(MdTester *t)
        MdTesterIterator testers()


cdef extern from "medic/visitor.h" namespace "MEDIC":
    cdef cppclass MdVisitor:
        MdVisitor()
        void visitTester(MdTester *tester)
        void visitNode(MdTester *tester, MdNode* node)
        void visitContext(MdTester *tester, MdContext* context)
        std_vector[MdTester *] reportTesters()
        MdReportIterator report(MdTester *tester)
        bint hasError(MdTester *tester)
        void reset()
        MdNodeIterator nodes()
        void clearOptions()
        MdParamContainer* getOptions(string name)
        std_vector[string] getOptionKeys()
        bint setScene(MdContext *scene)
        MdContext *scene()
        bint addAsset(MdContext *asset)
        MdContextIterator assets()
        void clearScene()
        void clearAssets()
        bint selectionOnly() const
        void setSelectionOnly(bint v)
        void initializeTester(MdTester *tester)
        void finalizeTester(MdTester *tester)

cdef extern from "medic/pluginManager.h" namespace "MEDIC":
    cdef enum MdPluginLoadingStatus:
        MdLoadingFailure = 0
        MdLoadingSuccess
        MdExistsPlugin

    cdef cppclass MdPlugInManager:
        @staticmethod
        MdPlugInManager *Instance()
        std_vector[string] testerNames()
        MdTester *tester(string name)
        MdKarte *karte(string name)
        MdPluginLoadingStatus addTester(string pluginPath)
        MdPluginLoadingStatus addKarte(string name, string description, bint visible, std_vector[string] testerNames)
        void unload()
        
