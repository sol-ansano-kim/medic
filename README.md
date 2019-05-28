# Medic([日本語](https://github.com/sol-ansano-kim/medic/blob/master/README_ja.md))

* Medic is an open source data check tool for Autodesk Maya. You can add a Karte file to define a new checklist, or create a check plugin (Tester) with C ++ or Python.


<img src="https://github.com/sol-ansano-kim/medic/wiki/images/medic.png" width="480"> 
<br><br><br>

* Demo  
<br>

[![Alt text](https://github.com/sol-ansano-kim/medic/wiki/images/medic_youtube.png)](https://youtu.be/FjWmc0GHiL8)

<br>

* Also, Medic can be used as a [Pyblish](http://pyblish.com) plugin ([medicToPyblish](https://github.com/sol-ansano-kim/medicToPyblish))

<br><br>
# Usage

1. [Download](https://github.com/sol-ansano-kim/medic/releases) the package and extract it.　(Please refer to the [build](https://github.com/sol-ansano-kim/medic/wiki#build) if you build it yourself or there is no package)
2. Copy the unzipped contents to a module directory or add the directory path to the environment variable MAYA_MODULE_PATH

   Module directory path

    windows : C:/Users/%USERNAME%/Documents/Maya/<Maya Version>/modules  
    osx : ~/Library/Preferences/Autodesk/maya/<Maya Version>/modules  
    linux : ~/maya/<Maya Version>/modules  

3. Execute the following command in Maya. For details on the usage, see [Usage](https://github.com/sol-ansano-kim/medic/wiki#usage).


```python
import medicUI
medicUI.Show()
```

4. To create a new checklist(Karte), see [Karte](https://github.com/sol-ansano-kim/medic/wiki#karte).
5. To write a new check plugin(Tester), see [Custom Tester](https://github.com/sol-ansano-kim/medic/wiki#custom-tester).