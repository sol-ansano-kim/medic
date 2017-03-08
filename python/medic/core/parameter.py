from maya import OpenMaya
import copy


MdNull = 0
MdBool = 1
MdInt = 2
MdFloat = 3
MdString = 4
MdBoolArray = 5
MdIntArray = 6
MdFloatArray = 7
MdStringArray = 8
MdMObject = 9
MdMObjectArray = 10


class Parameter(object):
    def __init__(self, name, label, parameterType, defaultValue, action=None):
        super(Parameter, self).__init__()
        self.__name = name
        self.__label = label
        self.__type = MdNull
        self.__default_value = None
        self.__value = None
        self.__action = action
        self.__size = 0

        self.setType(parameterType)
        if defaultValue is not None:
            self.setDefault(defaultValue)

    def setType(self, ptype):
        self.__type = ptype
        self.__default_value = None

        if self.__type is MdMObject:
            self.__value = OpenMaya.MObject()

        elif self.__type is MdMObjectArray:
            self.__value = OpenMaya.MObjectArray()

        elif self.isArray():
            self.__value = []

        else:
            self.__value = None

        self.resize(0)

    def getType(self):
        return self.__type

    def setName(self, name):
        self.__name = name

    def getName(self):
        return self.__name

    def setLabel(self, label):
        self.__label = label

    def getLabel(self):
        return self.__label

    def set(self, value, index=0):
        if (not self.isArray()) and (index is not 0):
            return False

        if not self.isArray():
            self.__value = value
            return True

        if index >= self.__size:
            return False

        self.__value[index] = value
        return True

    def get(self, index=0):
        if (not self.isArray()) and (index is not 0):
            return None

        if not self.isArray():
            return self.__value

        return self.__value[index]

    def setDefault(self, value):
        self.__default_value = value

    def getDefault(self):
        return self.__default_value

    def resize(self, s):
        if not self.isArray():
            self.__size = 0
            return

        if self.__type is MdMObjectArray:
            self.__value.setLength(s)

        else:
            if self.__size > s:
                self.__value = self.__value[:s]
            elif self.__size < s:
                self.__value = self.__value + [None] * (s - self.__size)

        self.__size = s

    def size(self):
        return self.__size

    def isArray(self):
        return self.__type in [MdBoolArray, MdIntArray, MdFloatArray, MdStringArray, MdMObjectArray]

    def hasAction(self):
        return self.__action is not None

    def doAction(self):
        if self.hasAction():
            self.__action.run(self)


class ParameterParser(object):
    def __init__(self, parameter_list=[]):
        super(ParameterParser, self).__init__()
        self.__parameter_list = {}
        for param in parameter_list:
            self.__addParameter(param)

    def __addParameter(self, parameter):
        if not self.__parameter_list.has_key(parameter.getName()):
            self.__parameter_list[parameter.getName()] = parameter
            return True

        return False

    def getParameter(self, param_name):
        return self.__parameter_list.get(param_name, None)

    def parameters(self):
        return copy.copy(self.__parameter_list)

    def getDefault(self, param_name):
        parm = self.getParameter(param_name)
        if parm is None:
            return None

        return parm.getDefault()

    def get(self, param_name, index=0):
        parm = self.getParameter(param_name)
        if parm is None:
            return None

        return parm.get(index)

    def size(self, param_name):
        parm = self.getParameter(param_name)
        if parm is None:
            return 0

        return parm.size()
