#ifndef MEDIC_KARTE_INCLUDED
#define MEDIC_KARTE_INCLUDED


#include <string>
#include <vector>


namespace MEDIC
{
    class MdKarte
    {
        public:
            MdKarte(std::string name, std::string desc);
            ~MdKarte();
            std::string Name() const;
            std::string Description() const;

        private:
            std::string m_name;
            std::string m_description;
            std::vector<std::string> m_patterns;
    };
}


#endif