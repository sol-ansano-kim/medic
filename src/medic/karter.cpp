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

const std::vector<std::string>& MdKarte::patterns() const
{
    return m_patterns;
}

void MdKarte::addPattern(std::string pattern)
{
    m_patterns.push_back(pattern);
}