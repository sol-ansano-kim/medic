#ifndef CORE_PARAMETER_INCLUDED
#define CORE_PARAMETER_INCLUDED


#include <string>
#include <vector>
#include <cstdlib>
#include "core/action.h"


class Action;


template <typename T> class Array
{
    public:
        Array();
        ~Array();
        void clear();
        void resize(size_t size);
        bool set(size_t index, const T &v);
        bool get(size_t index, T &v) const;
        
    private:
        std::vector<T> m_datas;
        size_t m_size;
};


class Parameter
{
    public:
        enum MTypes
        {
            MNull = 0,
            MBool,
            MInt,
            MFloat,
            MString,
            MBoolArray,
            MIntArray,
            MFloatArray,
            MStringArray
        };

        Parameter();
        template <typename T>
        Parameter(std::string name, std::string label, MTypes type, T &defaultValue, Action *action=0);
        Parameter(const Parameter &p);
        Parameter &operator=(const Parameter &p);
        ~Parameter();
        bool setType(MTypes type);
        MTypes getType() const;
        void setName(std::string);
        std::string getName() const;
        void setLabel(std::string);
        std::string getLabel() const;
        bool set(const bool &v, size_t index=0);
        bool get(bool &v, size_t index=0) const;
        bool set(const int &v, size_t index=0);
        bool set(const long &v, size_t index=0);
        bool get(int &v, size_t index=0) const;
        bool set(const float &v, size_t index=0);
        bool set(const double &v, size_t index=0);
        bool get(float &v, size_t index=0) const;
        bool set(const std::string &v, size_t index=0);
        bool set(const char *v, size_t index=0);
        bool get(std::string &v, size_t index=0) const;
        bool setDefault(const bool &v);
        bool getDefault(bool &v) const;
        bool setDefault(const int &v);
        bool getDefault(int &v) const;
        bool setDefault(const long &v);
        bool getDefault(long &v) const;
        bool setDefault(const float &v);
        bool getDefault(float &v) const;
        bool setDefault(const double &v);
        bool getDefault(double &v) const;
        bool setDefault(const std::string &v);
        bool setDefault(const char *v);
        bool getDefault(std::string &v) const;
        bool resize(size_t s);
        size_t size() const;
        bool isArray() const;
        bool hasAction() const;
        void doAction();

    private:
        void copyFrom(const Parameter &p);
        void destroy();
        bool allocate();
        std::string m_name;
        std::string m_label;
        size_t m_size;
        size_t m_byte_size;
        bool m_is_array;
        MTypes m_type;
        Action *m_action;
        void *m_default;
        void *m_value;
};

template <typename T>
Parameter::Parameter(std::string name, std::string label, MTypes type, T &defaultValue, Action *action)
: m_name(name), m_label(label), m_value(0), m_default(0)
{
    setType(type);
    m_action = action;
    setDefault(defaultValue);
}


template <typename T>void CopyArrayValue(void *dst, void *src);
template <typename T>
bool SetValue(void *data, const T &v, size_t index, bool isArray);
template <typename T>
bool GetValue(void *data, T &v, size_t index, bool isArray);


#endif