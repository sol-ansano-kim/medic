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
        Parameter(std::string name, std::string label, MTypes type);
        Parameter(std::string name, std::string label, MTypes type, Action *action);
        Parameter(const Parameter &p);
        Parameter &operator=(const Parameter &p);
        ~Parameter();
        bool setType(MTypes type);
        MTypes getType() const;
        void setName(std::string);
        std::string getName() const;
        void setLabel(std::string);
        std::string getLabel() const;
        template <typename T>
        bool set(const T &v, size_t index=0);
        template <typename T>
        bool get(T &v, size_t index=0) const;
        template <typename T>
        bool setDefault(const T &v);
        template <typename T>
        bool getDefault(T &v) const;
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


template <typename T>void CopyArrayValue(void *dst, void *src);


#endif