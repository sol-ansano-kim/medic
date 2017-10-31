#ifndef MEDIC_KARTE_INCLUDED
#define MEDIC_KARTE_INCLUDED


#include <string>
#include <vector>


namespace MEDIC
{
    class MdKarte
    {
        public:
            MdKarte(const std::string &name, const std::string &desc);
            MdKarte(const std::string &name, const std::string &desc, const std::string &file);
            ~MdKarte();
            const std::string &Name() const;
            const std::string &Description() const;
            const std::string &File() const;
            const std::vector<std::string>& patterns() const;
            void addPattern(std::string pattern);

        private:
            std::string m_name;
            std::string m_description;
            std::string m_file;
            std::vector<std::string> m_patterns;
    };
}


#endif