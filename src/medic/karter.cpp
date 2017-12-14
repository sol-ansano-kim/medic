#include "medic/karte.h"


using namespace MEDIC;


MdKarte::MdKarte(const std::string &name, const std::string &desc)
    : m_name(name), m_description(desc) {}

MdKarte::MdKarte(const std::string &name, const std::string &desc, const std::string &file)
    : m_name(name), m_description(desc), m_file(file) {}

MdKarte::~MdKarte() {}

const std::string &MdKarte::Name() const
{
    return m_name;
}

const std::string &MdKarte::Description() const
{
    return m_description;
}

const std::string &MdKarte::File() const
{
    return m_file;
}

const std::vector<const std::string>& MdKarte::patterns() const
{
    return m_patterns;
}

void MdKarte::addPattern(const std::string pattern)
{
    std::vector<const std::string>::iterator it = std::find(m_patterns.begin(), m_patterns.end(), pattern);
    if (it == m_patterns.end())
    {
        m_patterns.push_back(pattern);
    }
}