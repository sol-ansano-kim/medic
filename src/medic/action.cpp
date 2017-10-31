#include "medic/action.h"


using namespace MEDIC;


MdAction::MdAction() {}

MdAction::~MdAction() {}

MdAction *MdAction::Create() const
{
    return new MdAction();
}