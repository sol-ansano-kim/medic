#ifndef MEDIC_PARAMETER_INCLUDED
#define MEDIC_PARAMETER_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include "medic/action.h"


namespace MEDIC
{
    class MdAction;
    class MdParameter;
    typedef std::map<std::string, MdParameter *> ParamPtrMap;

    template <typename T> class MdArray
    {
        public:
            MdArray();
            ~MdArray();
            void clear();
            void resize(size_t size);
            bool set(size_t index, const T &v);
            bool get(size_t index, T &v) const;

        private:
            std::vector<T> m_datas;
            size_t m_size;
    };

    enum MdTypes
    {
        MdNull = 0,
        MdBool,
        MdInt,
        MdFloat,
        MdString,
        MdBoolArray,
        MdIntArray,
        MdFloatArray,
        MdStringArray
    };


    class MdParameter
    {
        public:
            MdParameter();
            template <typename T>
            MdParameter(std::string name, std::string label, MdTypes type, const T &defaultValue, MdAction *action=0);
            template <typename T>
            static MdParameter *Create(std::string name, std::string label, MdTypes type, const T &defaultValue, MdAction *action=0);
            MdParameter(const MdParameter &p);
            MdParameter &operator=(const MdParameter &p);
            ~MdParameter();
            bool setType(MdTypes type);
            MdTypes getType() const;
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
            void copyFrom(const MdParameter &p);
            void destroy();
            bool allocate();
            std::string m_name;
            std::string m_label;
            void *m_value;
            void *m_default;
            size_t m_size;
            MdTypes m_type;
            MdAction *m_action;
    };

    template <typename T>
    MdParameter::MdParameter(std::string name, std::string label, MdTypes type, const T &defaultValue, MdAction *action)
    : m_name(name), m_label(label), m_value(0), m_default(0)
    {
        setType(type);
        m_action = action;
        setDefault(defaultValue);
    }

    template <typename T>
    MdParameter *MdParameter::Create(std::string name, std::string label, MdTypes type, const T &defaultValue, MdAction *action)
    {
        return new MdParameter(name, label, type, defaultValue, action);
    }


    class MdParamContainer
    {
        public:
            MdParamContainer();
            ~MdParamContainer();
            bool append(MdParameter *p);
            bool remove(MdParameter *p);
            MdParameter *getParam(std::string name);
            void clear();
            size_t size() const;
            std::vector<std::string> names();

            template <typename T>
            bool set(std::string paramName, const T &v, size_t index=0);
            template <typename T>
            bool get(std::string paramName, T &v, size_t index=0);
            template <typename T>
            bool getDefault(std::string paramName, T &v);

        private:
            ParamPtrMap m_params;
    };

    template <typename T>
    bool MdParamContainer::set(std::string paramName, const T &v, size_t index)
    {
        MdParameter *p = getParam(paramName);
        if (p)
        {
            return p->set(v, index);
        }

        return false;
    }

    template <typename T>
    bool MdParamContainer::get(std::string paramName, T &v, size_t index)
    {
        MdParameter *p = getParam(paramName);
        if (p)
        {
            return p->get(v, index);
        }

        return false;
    }

    template <typename T>
    bool MdParamContainer::getDefault(std::string paramName, T &v)
    {
        MdParameter *p = getParam(paramName);
        if (p)
        {
            return p->getDefault(v);
        }

        return false;
    }
}


template <typename T>
void CopyArrayValue(void *dst, void *src);
template <typename T>
bool SetValue(void *data, const T &v, size_t index, bool isArray);
template <typename T>
bool GetValue(void *data, T &v, size_t index, bool isArray);

#endif // MEDIC_PARAMETER_INCLUDED