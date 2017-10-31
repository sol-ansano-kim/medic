#include "medic/karte.h"


using namespace MEDIC;


MdKarte::MdKarte(std::string name, std::string desc) : m_name(name), m_description(desc) {}

MdKarte::~MdKarte() {}

std::string MdKarte::Name() const
{
    return m_name;
}

std::string MdKarte::Description() const
{
    return m_description;
}
