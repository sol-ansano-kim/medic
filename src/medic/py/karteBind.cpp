#include "medic/py/karteBind.h"


using namespace pybind11;
using namespace MEDIC;


void bindKarte(pybind11::module mod)
{
    class_<MdKarte, std::unique_ptr<MdKarte, nodelete>>(mod, "Karte")
        .def("Name", &MdKarte::Name)
        .def("Description", &MdKarte::Description)
        .def("File", &MdKarte::File);
}
