#include "core/parameter.h"


template <typename T> Array<T>::Array()
: m_size(0) {}

template <typename T> Array<T>::~Array()
{
    clear();
}

template <typename T> void Array<T>::clear()
{
    m_datas.clear();
}

template <typename T> void Array<T>::resize(size_t size)
{
    m_size = size;
    m_datas.resize(m_size);
}

template <typename T> bool Array<T>::set(size_t index, const T &v)
{
    if (index >= m_size)
    {
        return false;
    }

    m_datas[index] = v;
    return true;
}

template <typename T> bool Array<T>::get(size_t index, T &v) const
{
    if (index >= m_size)
    {
        return false;
    }

    v = m_datas[index];
    return true;
}


Parameter::Parameter()
: m_name(""), m_label(""), m_value(0), m_default(0), m_size(0), m_type(MNull), m_action(0) {}

Parameter::Parameter(std::string name, std::string label, MTypes type)
: m_name(name), m_label(label), m_value(0), m_default(0), m_action(0)
{
    setType(type);
}

Parameter::Parameter(std::string name, std::string label, MTypes type, Action *action)
: m_name(name), m_label(label), m_value(0), m_default(0)
{
    setType(type);
    m_action = action;
}

Parameter &Parameter::operator=(const Parameter &p)
{
    copyFrom(p);
    return *this;
}

void Parameter::copyFrom(const Parameter &p)
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
        case (MBool):
            std::memcpy(m_default, p.m_default, sizeof(bool));
            std::memcpy(m_value, p.m_value, sizeof(bool));
            break;
        case (MInt):
            std::memcpy(m_default, p.m_default, sizeof(int));
            std::memcpy(m_value, p.m_value, sizeof(int));
            break;
        case (MFloat):
            std::memcpy(m_default, p.m_default, sizeof(float));
            std::memcpy(m_value, p.m_value, sizeof(float));
            break;
        case (MString):
            std::memcpy(m_default, p.m_default, sizeof(std::string));
            std::memcpy(m_value, p.m_value, sizeof(std::string));
            break;
        case (MBoolArray):
            std::memcpy(m_default, p.m_default, sizeof(bool));
            CopyArrayValue<bool>(m_value, p.m_value);
            break;
        case (MIntArray):
            std::memcpy(m_default, p.m_default, sizeof(int));
            CopyArrayValue<int>(m_value, p.m_value);
            break;
        case (MFloatArray):
            std::memcpy(m_default, p.m_default, sizeof(float));
            CopyArrayValue<float>(m_value, p.m_value);
            break;
        case (MStringArray):
            std::memcpy(m_default, p.m_default, sizeof(std::string));
            CopyArrayValue<std::string>(m_value, p.m_value);
            break;
        default:
            break;
    }
}

Parameter::Parameter(const Parameter &p)
{
    copyFrom(p);
}

Parameter::~Parameter()
{
    destroy();
}

bool Parameter::setType(MTypes type)
{
    destroy();
    m_type = type;

    return allocate();
}

Parameter::MTypes Parameter::getType() const
{
    return m_type;
}

void Parameter::setName(std::string name)
{
    m_name = name;
}

std::string Parameter::getName() const
{
    return m_name;
}

void Parameter::setLabel(std::string label)
{
    m_label = label;
}

std::string Parameter::getLabel() const
{
    return m_label;
}

template <typename T>
bool Parameter::set(const T &v, size_t index)
{
    if (!isArray() && index != 0)
    {
        return false;
    }

    if (!isArray())
    {
        *((T *)m_value) = v;
        return true;
    }

    return ((Array<T>*)m_value)->set(index, v);
}

template <typename T>
bool Parameter::get(T &v, size_t index) const
{
    if (!isArray() && index != 0)
    {
        return false;
    }

    if (!isArray())
    {
        v = *((T *)m_value);
        return true;
    }

    return ((Array<T>*)m_value)->get(index, v);
}

template <typename T>
bool Parameter::setDefault(const T &v)
{
    *((T *)m_default) = v;
    return true;
}

template <typename T>
bool Parameter::getDefault(T &v) const
{
    v = *((T *)m_default);
    return true;
}

bool Parameter::resize(size_t s)
{
    if (!isArray())
    {
        m_size = 1;
        return false;
    }

    switch (m_type)
    {
        case (MBoolArray):
            ((Array<bool> *)m_value)->resize(s);
            break;
        case (MIntArray):
            ((Array<int> *)m_value)->resize(s);
            break;
        case (MFloatArray):
            ((Array<float> *)m_value)->resize(s);
            break;
        case (MStringArray):
            ((Array<std::string> *)m_value)->resize(s);
            break;
        default:
            return false;
            break;
    }

    m_size = s;

    return true;
}

size_t Parameter::size() const
{
    return m_size;
}

bool Parameter::isArray() const
{
    return (m_type == MBoolArray || m_type == MIntArray || m_type == MFloatArray || m_type == MStringArray);
}

bool Parameter::hasAction() const
{
    return (m_action != 0);
}

void Parameter::doAction()
{
    if (hasAction())
    {
        m_action->run(this);    
    }
}

void Parameter::destroy()
{
    if (m_value != 0)
    {
        switch (m_type)
        {
            case (MBool):
                delete (bool *)m_value;
                break;
            case (MInt):
                delete (int *)m_value;
                break;
            case (MFloat):
                delete (float *)m_value;
                break;
            case (MString):
                delete (std::string *)m_value;
                break;
            case (MBoolArray):
                delete (Array<bool> *)m_value;
                break;
            case (MIntArray):
                delete (Array<int> *)m_value;
                break;
            case (MFloatArray):
                delete (Array<float> *)m_value;
                break;
            case (MStringArray):
                delete (Array<std::string> *)m_value;
                break;

            default:
                free(m_value);
                break;
        }
    }
    if (m_default != 0)
    {
        switch (m_type)
        {
            case (MBool):
            case (MBoolArray):
                delete (bool *)m_default;
                break;
            case (MInt):
            case (MIntArray):
                delete (int *)m_default;
                break;
            case (MFloat):
            case (MFloatArray):
                delete (float *)m_default;
                break;
            case (MString):
            case (MStringArray):
                delete (std::string *)m_default;
                break;
            default:
                free(m_default);
                break;
        }
    }
}

bool Parameter::allocate()
{
    switch (m_type)
    {
        case (MBool):
            m_default = new bool();
            m_value = new bool();
            break;
        case (MInt):
            m_default = new int();
            m_value = new int();
            break;
        case (MFloat):
            m_default = new float();
            m_value = new float();
            break;
        case (MString):
            m_default = new std::string();
            m_value = new std::string();
            break;
        case (MBoolArray):
            m_default = new bool();
            m_value = new Array<bool>();
            break;
        case (MIntArray):
            m_default = new int();
            m_value = new Array<int>();
            break;
        case (MFloatArray):
            m_default = new float();
            m_value = new Array<float>();
            break;
        case (MStringArray):
            m_default = new std::string();
            m_value = new Array<std::string>();
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


template <typename T>void CopyArrayValue(void *dst, void *src)
{
    std::vector<T> *src_vector = (std::vector<T> *)src;
    std::vector<T> *dst_vector = (std::vector<T> *)dst;
    for (size_t i = 0; i < src_vector->size(); ++i)
    {
        dst_vector->at(i) = src_vector->at(i);
    }
}