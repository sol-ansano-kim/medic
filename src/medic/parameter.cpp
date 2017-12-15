#include "medic/parameter.h"


using namespace MEDIC;


template <typename T> MdArray<T>::MdArray()
    : m_size(0) {}

template <typename T> MdArray<T>::~MdArray()
{
    clear();
}

template <typename T> void MdArray<T>::clear()
{
    m_datas.clear();
}

template <typename T> void MdArray<T>::resize(size_t size)
{
    m_size = size;
    m_datas.resize(m_size);
}

template <typename T> bool MdArray<T>::set(size_t index, const T &v)
{
    if (index >= m_size)
    {
        return false;
    }

    m_datas[index] = v;
    return true;
}

template <typename T> bool MdArray<T>::get(size_t index, T &v) const
{
    if (index >= m_size)
    {
        return false;
    }

    v = m_datas[index];
    return true;
}


MdParameter::MdParameter()
    : m_name(""), m_label(""), m_value(0), m_default(0), m_size(0), m_type(MdNull), m_action(0) {}

MdParameter &MdParameter::operator=(const MdParameter &p)
{
    copyFrom(p);
    return *this;
}

void MdParameter::copyFrom(const MdParameter &p)
{
    m_name = p.m_name;
    m_label = p.m_label;

    setType(p.m_type);

    if (isArray())
    {
        resize(p.m_size);
    }

    if (p.m_action != 0)
    {
        m_action = p.m_action->Create();
    }

    switch (m_type)
    {
        case (MdBool):
            std::memcpy(m_default, p.m_default, sizeof(bool));
            std::memcpy(m_value, p.m_value, sizeof(bool));
            break;
        case (MdInt):
            std::memcpy(m_default, p.m_default, sizeof(int));
            std::memcpy(m_value, p.m_value, sizeof(int));
            break;
        case (MdFloat):
            std::memcpy(m_default, p.m_default, sizeof(float));
            std::memcpy(m_value, p.m_value, sizeof(float));
            break;
        case (MdString):
            std::memcpy(m_default, p.m_default, sizeof(std::string));
            std::memcpy(m_value, p.m_value, sizeof(std::string));
            break;
        case (MdBoolArray):
            std::memcpy(m_default, p.m_default, sizeof(bool));
            CopyArrayValue<bool>(m_value, p.m_value);
            break;
        case (MdIntArray):
            std::memcpy(m_default, p.m_default, sizeof(int));
            CopyArrayValue<int>(m_value, p.m_value);
            break;
        case (MdFloatArray):
            std::memcpy(m_default, p.m_default, sizeof(float));
            CopyArrayValue<float>(m_value, p.m_value);
            break;
        case (MdStringArray):
            std::memcpy(m_default, p.m_default, sizeof(std::string));
            CopyArrayValue<std::string>(m_value, p.m_value);
            break;
        default:
            break;
    }
}

MdParameter::MdParameter(const MdParameter &p)
{
    copyFrom(p);
}

MdParameter::~MdParameter()
{

    destroy();
}

bool MdParameter::setType(MdParamType type)
{
    destroy();
    m_type = type;

    return allocate();
}

MdParamType MdParameter::getType() const
{
    return m_type;
}

void MdParameter::setName(std::string name)
{
    m_name = name;
}

std::string MdParameter::getName() const
{
    return m_name;
}

void MdParameter::setLabel(std::string label)
{
    m_label = label;
}

std::string MdParameter::getLabel() const
{
    return m_label;
}

bool MdParameter::set(const bool &v, size_t index)
{
    return SetValue<bool>(m_value, v, index, isArray());
}

bool MdParameter::set(const int &v, size_t index)
{
    return SetValue<int>(m_value, v, index, isArray());
}

bool MdParameter::set(const long &v, size_t index)
{
    return SetValue<int>(m_value, (int)v, index, isArray());
}

bool MdParameter::set(const float &v, size_t index)
{
    return SetValue<float>(m_value, v, index, isArray());
}

bool MdParameter::set(const double &v, size_t index)
{
    return SetValue<float>(m_value, (float)v, index, isArray());
}

bool MdParameter::set(const std::string &v, size_t index)
{
    return SetValue<std::string>(m_value, v, index, isArray());
}

bool MdParameter::set(const char *v, size_t index)
{
    return SetValue<std::string>(m_value, (std::string)v, index, isArray());
}

bool MdParameter::get(bool &v, size_t index) const
{
    return GetValue<bool>(m_value, v, index, isArray());
}

bool MdParameter::get(int &v, size_t index) const
{
    return GetValue<int>(m_value, v, index, isArray());
}

bool MdParameter::get(float &v, size_t index) const
{
    return GetValue<float>(m_value, v, index, isArray());
}

bool MdParameter::get(std::string &v, size_t index) const
{
    return GetValue<std::string>(m_value, v, index, isArray());
}

bool MdParameter::setDefault(const bool &v)
{
    return SetValue(m_default, v, 0, false);
}
bool MdParameter::getDefault(bool &v) const
{
    return GetValue(m_default, v, 0, false);
}
bool MdParameter::setDefault(const int &v)
{
    return SetValue(m_default, v, 0, false);
}
bool MdParameter::getDefault(int &v) const
{
    return GetValue(m_default, v, 0, false);
}
bool MdParameter::setDefault(const long &v)
{
    return SetValue(m_default, v, 0, false);
}
bool MdParameter::getDefault(long &v) const
{
    return GetValue(m_default, v, 0, false);
}
bool MdParameter::setDefault(const float &v)
{
    return SetValue(m_default, v, 0, false);
}
bool MdParameter::getDefault(float &v) const
{
    return GetValue(m_default, v, 0, false);
}
bool MdParameter::setDefault(const double &v)
{
    return SetValue(m_default, v, 0, false);
}
bool MdParameter::getDefault(double &v) const
{
    return GetValue(m_default, v, 0, false);
}
bool MdParameter::setDefault(const std::string &v)
{
    return SetValue(m_default, v, 0, false);
}

bool MdParameter::setDefault(const char *v)
{
    return SetValue(m_default, (std::string)v, 0, false);
}

bool MdParameter::getDefault(std::string &v) const
{
    return GetValue(m_default, v, 0, false);
}

bool MdParameter::resize(size_t s)
{
    if (!isArray())
    {
        m_size = 1;
        return false;
    }

    switch (m_type)
    {
        case (MdBoolArray):
            ((MdArray<bool> *)m_value)->resize(s);
            break;
        case (MdIntArray):
            ((MdArray<int> *)m_value)->resize(s);
            break;
        case (MdFloatArray):
            ((MdArray<float> *)m_value)->resize(s);
            break;
        case (MdStringArray):
            ((MdArray<std::string> *)m_value)->resize(s);
            break;
        default:
            return false;
            break;
    }

    m_size = s;

    return true;
}

size_t MdParameter::size() const
{
    return m_size;
}

bool MdParameter::isArray() const
{
    return (m_type == MdBoolArray || m_type == MdIntArray || m_type == MdFloatArray || m_type == MdStringArray);
}

bool MdParameter::hasAction() const
{
    return (m_action != 0);
}

void MdParameter::doAction()
{
    if (hasAction())
    {
        m_action->run(this);    
    }
}

void MdParameter::destroy()
{
    if (m_value != 0)
    {
        switch (m_type)
        {
            case (MdBool):
                delete (bool *)m_value;
                break;
            case (MdInt):
                delete (int *)m_value;
                break;
            case (MdFloat):
                delete (float *)m_value;
                break;
            case (MdString):
                delete (std::string *)m_value;
                break;
            case (MdBoolArray):
                delete (MdArray<bool> *)m_value;
                break;
            case (MdIntArray):
                delete (MdArray<int> *)m_value;
                break;
            case (MdFloatArray):
                delete (MdArray<float> *)m_value;
                break;
            case (MdStringArray):
                delete (MdArray<std::string> *)m_value;
                break;

            default:
                free(m_value);
                break;
        }
    }

    m_value = 0;

    if (m_default != 0)
    {
        switch (m_type)
        {
            case (MdBool):
            case (MdBoolArray):
                delete (bool *)m_default;
                break;
            case (MdInt):
            case (MdIntArray):
                delete (int *)m_default;
                break;
            case (MdFloat):
            case (MdFloatArray):
                delete (float *)m_default;
                break;
            case (MdString):
            case (MdStringArray):
                delete (std::string *)m_default;
                break;
            default:
                free(m_default);
                break;
        }
    }

    m_default = 0;
}

bool MdParameter::allocate()
{
    switch (m_type)
    {
        case (MdBool):
            m_default = new bool();
            m_value = new bool();
            break;
        case (MdInt):
            m_default = new int();
            m_value = new int();
            break;
        case (MdFloat):
            m_default = new float();
            m_value = new float();
            break;
        case (MdString):
            m_default = new std::string();
            m_value = new std::string();
            break;
        case (MdBoolArray):
            m_default = new bool();
            m_value = new MdArray<bool>();
            break;
        case (MdIntArray):
            m_default = new int();
            m_value = new MdArray<int>();
            break;
        case (MdFloatArray):
            m_default = new float();
            m_value = new MdArray<float>();
            break;
        case (MdStringArray):
            m_default = new std::string();
            m_value = new MdArray<std::string>();
            break;
        default:
            break;
    }

    if (!m_value)
    {
        return false;
    }

    return true;
}


MdParamContainer::MdParamContainer() {}

MdParamContainer::~MdParamContainer()
{
    clear();
}

bool MdParamContainer::append(MdParameter *p)
{
    ParamPtrMap::iterator it = m_params.find(p->getName());
    if (it == m_params.end())
    {
        m_params[p->getName()] = p;
        return true;
    }

    return false;
}

bool MdParamContainer::remove(MdParameter *p)
{
    ParamPtrMap::iterator it = m_params.find(p->getName());
    if (it != m_params.end())
    {
        delete it->second;
        m_params.erase(it);
        return true;
    }

    return false;
}

void MdParamContainer::clear()
{
    for (ParamPtrMap::iterator it = m_params.begin(); it != m_params.end(); ++it)
    {
        delete it->second;
    }

    m_params.clear();
}

size_t MdParamContainer::size() const
{
    return m_params.size();
}

MdParameter *MdParamContainer::getParam(std::string name)
{
    ParamPtrMap::iterator it = m_params.find(name);
    if (it != m_params.end())
    {
        return it->second;
    }

    return 0;
}

bool MdParamContainer::hasParam(std::string name)
{
    ParamPtrMap::iterator it = m_params.find(name);

    return it != m_params.end();
}

std::vector<std::string> MdParamContainer::names()
{
    std::vector<std::string> param_names;
    for (ParamPtrMap::iterator it = m_params.begin(); it != m_params.end(); ++it)
    {
        param_names.push_back(it->first);
    }

    return param_names;
}

bool MdParamContainer::getBool(std::string paramName, size_t index)
{
    bool v = false;
    get(paramName, v, index);

    return v;
}

int MdParamContainer::getInt(std::string paramName, size_t index)
{
    int v = 0;
    get(paramName, v, index);

    return v;
}

float MdParamContainer::getFloat(std::string paramName, size_t index)
{
    float v = 0.f;
    get(paramName, v, index);

    return v;
}

std::string MdParamContainer::getString(std::string paramName, size_t index)
{
    std::string v("");
    get(paramName, v, index);

    return v;
}

bool MdParamContainer::getDefaultBool(std::string paramName)
{
    bool v = false;
    getDefault(paramName, v);

    return v;
}

int MdParamContainer::getDefaultInt(std::string paramName)
{
    int v = 1;
    getDefault(paramName, v);

    return v;
}

float MdParamContainer::getDefaultFloat(std::string paramName)
{
    float v = 0.0f;
    getDefault(paramName, v);

    return v;
}

std::string MdParamContainer::getDefaultString(std::string paramName)
{
    std::string v("");
    getDefault(paramName, v);

    return v;
}

template <typename T>void CopyArrayValue(void *dst, void *src)
{
    std::vector<T> *src_vector = (std::vector<T> *)src;
    std::vector<T> *dst_vector = (std::vector<T> *)dst;
    for (size_t i = 0; i < src_vector->size(); ++i)
    {
        dst_vector->at(i) = src_vector->at(i);
    }
}

template <typename T>
bool SetValue(void *data, const T &v, size_t index, bool isArray)
{
    if (data == 0)
    {
        return false;
    }

    if (!isArray && index != 0)
    {
        return false;
    }

    if (!isArray)
    {
        *((T *)data) = v;
        return true;
    }

    return ((MdArray<T>*)data)->set(index, v);
}

template <typename T>
bool GetValue(void *data, T &v, size_t index, bool isArray)
{
    if (data == 0)
    {
        return false;
    }

    if (!isArray && index != 0)
    {
        return false;
    }

    if (!isArray)
    {
        v = *((T *)data);
        return true;
    }

    return ((MdArray<T>*)data)->get(index, v);
}