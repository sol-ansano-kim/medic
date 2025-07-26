# 1.5.5
- Support maya2025 and maya2026

# 1.5.4
- Support maya2023 and maya2024

# 1.5.3
- Support maya2022 and python3
- Use PySide2 directly instead of Qt.py. Versions prior to maya2016 are no longer supported

# 1.5.2
- Sort Karte and Tester list

# 1.5.1
- Fix missing isAlive method of Report class

# 1.5.0
- Fix crash issue when node deleted.

# 1.4.0
- Add public API GetTester, GetKarte, GetTesterNames, GetKarteNames, ReloadPlugins.

```python
import medic
medic.GetKarte("All")
medic.GetTester("NGon")
print(medic.GetTesterNames())
print(medic.GetKarteNames())
medic.ReloadPlugins()

```

- Add new Visitor methods visitKarte, visitTester, visitNode, visitContext and with these test logic can be overriden.

default visitor class works like this.
```
visitKarte
   for each tester:
       visitTester(tester)

visitTester
   for each node:
       visitNode(tester, node)

   for each context:
       visitContext(tester, context)

```
```python
import medic

class MyCustomVisitor(medic.Visitor):
    def __init__(self):
        super(MyCustomVisitor, self).__init__()

    def visitNode(self, tester, node):
        print("Tester is '{}' and Node is '{}'".format(node.name()))
        super(MyCustomVisitor, self).visitNode(tester, node)

v = MyCustomVisitor()
v.visitKarte(medic.Karte("All"))

# GUI
import medicUI
medicUI.Show(visitorClass=MyCustomVisitor)
```

- Add a new Visitor method canVisit(karte=None, tester=None, node=None, context=None) to filter visiting by client
```python
class MyCustomVisitor(medic.Visitor):
    def __init__(self):
        super(MyCustomVisitor, self).__init__()

    def canVisit(self, karte=None, tester=None, node=None, context=None):
        if node is not None and node.type() == "mesh":
            return False

        return True
```

# 1.3.0
- Do not process templated dags
- Allow to start GUI with specified karte set.
- Add showAllKartes toggle button.
- support Maya 2020

# 1.2.4
- Fix crash problem when selection only mode is on (ignore kControllerTag)

# 1.2.3
- Add collecting mode button (ALL/SEL)

# 1.2.2

## Test Selected Nodes
- Support to test only with selected node tree
```python
Visitor.selectionOnly()
Visitor.setSelectionOnly(bool)
```

## Logger
- Now log level can be set (medic.LogDebug, medic.LogWarning, medic.LogError)
```python
import medic
medic.SetLogLevel(logLevel)
```

## Bug fix
- UnFrozenTransforms now works correctly


# 1.2.1

## Karte
- medicUI does not display the karte that "Visible" is  False.

e.g.
```
{
    "Name": "Hidden",
    "Description": "hidden karte",
    "Testers": ["*"],
    "Visible": False
}
```

## Buf fix
- UnFrozenTransforms

# 1.2.0

## Tester Renamed
HasLayer -> AnyLayer  
HasInputGeometry -> ConstructionHistory  
NotDefaultUVSet -> CurrentUVSet  
IncompleteUV -> IncompleteUVSet  
InstanceNode -> InstancedNode  
OverFourSidedPolygon -> NGon  
HasNamespace -> Namespace  
Referenced -> ReferencedNode  
ShapeNamedAfterTransform -> ShapeSuffix  
NotFreezed -> UnFrozenTransforms  
FreezeVertex -> VertexTweaks  
EdgeLengthZero -> ZeroLengthEdge  


## Tester Dependency
- Testers now have dependencies

```c++
std::vector<std::string> MdTester::Dependencies()
```
```
(list of string) Tester.Dependencies()
```

Tester will not run unless dependencies succeed.

## Tester Initialization, Finalization
- Tester now have initialize(), finalize() methods

```
<initialize>
<node loop>
{
    <test>
}
<finalize>
```

```c++
void MdTester::initialize()
void MdTester::finalize()
```
```
Tester.initialize()
Tester.finalize()
```

## Overriding UI Visitor
- Now Visitor used in medicUI can be overriden by visitorClass option.

```python
medicUI.Show(visitorClass=CustomVisitor)
```

## MdContext
- MdContext is a new test unit for Asset, Scene scope and it contains a MdParamContainer. 


```c++
MdContext(const std::string& name)
MdContext(const std::string& name, MdParamContainer* params)
std::string MdContext::name() const;
MdParamContainer* MdContext::params();
MdParameter* MdContext::param(const std::string& name);
bool MdContext::addParam(MdParameter* param);
```

```python
(Context) Context.Create(string)
(string) Context.name()
(ParamContainer) Context.params()
```
And Tester::test, Tester::Match, MdReport() accept it.

```c++
bool MdTester::Match(MdContext *context);
MdReport *MdTester::test(MdContext *node);

MdReport::MdReport(MdContext* context);
MdContext *MdReport::context();
```

## Tester Scope
- Testers are distinguished by MdTesterScope(MdAssetTester, MdSceneTester, MdNodeTester).

```c++
MdTesterScope MdTester::Scope()

e.g.
MdTesterScope CustomTester::Scope()
{
    return MdAssetTester;
}
```

```python
(TesterScope) Tester.Scope()

e.g.
    .....
    def Scope(self):
        return medic.TesterScopes.AssetTester

```

Asset and Scene are generated and added from Visitor.

```c++
bool MdVisitor::setScene(MdContext *scene)
MdContext *MdVisitor::scene()
bool MdVisitor::addAsset(MdContext *asset)
MdContextIterator MdVisitor::assets()
void MdVisitor::clearScene()
void MdVisitor::clearAssets()
```

```
(bool) Visitor.setScene(Context)
(Context) Visitor.scene()
(bool) Visitor.addAsset(Context)
(list of Context) Visitor.assets()
Visitor.clearScene()
Visitor.clearAssets()
```

- collectScene, collectAssets methods will be called when necessary, it will be usefull when using an inherited visitor class (python only)

e.g.

```python
import medic
import medicUI
from maya import cmds

class CustomVisitor(medic.Visitor):
    def __init__(self):
        super(CustomVisitor, self).__init__()

    def collectScene(self):
        scn = medic.Context.Create("scene")
        params = scn.params()
        params.append(medic.Parameter.Create("path", "path", medic.Types.String, ""))
        params.set("path", cmds.file(q=True, sn=True))
        self.setScene(scn)

    def collectAssets(self):
        for i in range(10):
            asset = medic.Context.Create("asset_{}".format(i))
            params = asset.params()
            params.append(medic.Parameter.Create("index", "index", medic.Types.Int, 0))
            params.set("index", i)
            self.addAsset(asset)


medicUI.Show(CustomVisitor)
```

## Options
- Visitor accepts Options for Tester and distributes to each Tester.

```
void MdVisitor::clearOptions();
MdParamContainer* MdVisitor::getOptions(const std::string& name);
std::vector<std::string> MdVisitor::getOptionKeys();
```

```
(dict) Visitor.getOptions()
Visitor.setOptions(dict)
```

Tester can access to the options declared with its name.

```c++
MdParamContainer* MdTester::getOptions() const
```

```
(ParamContainer) Tester.getOptions()
```
e.g.

```python
v = medic.Visitor()
v.setOptions({"ConstructionHistory": {"testBool": False}})
```

```c++
MdParamContainer* options = getOptions();

bool testBool;
if (options.get("testBool", testBool))
{
    MGlobal::displayInfo("OK");
}
```

```python
options = self.getOptions()
if "testBool" in options.names():
    print(options.get("testBool"))
```

## Tester Improvement
- FaceAssigned checks instObjGroups.objectGroups also
- UnFrozenTransforms is now fixable
- NotFreezed now checks all transform nodes
- Deleting node uses maya.cmds.delete


# 1.1.2
## Testers
- NotFreezed - Check all transform nodes