#include "core/action.h"

Action::Action() {}

Action::~Action() {}

Action *Action::Create()
{
    return new Action();
}