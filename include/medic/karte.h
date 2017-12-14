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
            const std::vector<const std::string>& patterns() const;
            void addPattern(const std::string pattern);

        private:
            const std::string m_name;
            const std::string m_description;
            const std::string m_file;
            std::vector<const std::string> m_patterns;
    };
}


#endif // MEDIC_KARTE_INCLUDED