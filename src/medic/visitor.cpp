#include "medic/visitor.h"


using namespace MEDIC;


MdVisitor::MdVisitor(MdKarte *karte) : m_karte(karte) {}

MdVisitor::~MdVisitor()
{
    reset();
}

void MdVisitor::visit()
{
    reset();
}

void MdVisitor::visit(MdTester *tester) {}

void MdVisitor::reset() {}
