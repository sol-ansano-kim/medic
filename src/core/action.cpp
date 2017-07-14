#include "medic/action.h"


using namespace MEDIC;


MdAction::MdAction() {}

MdAction::~MdAction() {}

MdAction *MdAction::Create()
{
    return new MdAction();
}