#include "medic/debug.h"


using namespace MEDIC;


void MEDIC::warn(std::string text)
{
    #ifdef _DEBUG
    std::cout << "[MEDIC] Warning - " << text << std::endl;
    #endif // _DEBUG
}

void MEDIC::debug(std::string text)
{
    #ifdef _DEBUG
    std::cout << "[MEDIC] Debug - " << text << std::endl;
    #endif // _DEBUG
}

void MEDIC::error(std::string text)
{
    #ifdef _DEBUG
    std::cout << "[MEDIC] Error - " << text << std::endl;
    #endif // _DEBUG
}
