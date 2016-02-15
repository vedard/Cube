#include "sprinter.h"

Sprinter::Sprinter() :Monster(M_SPRINTER)
{
	m_health = 80;
	m_AttackSpeed = .5f;
}
Sprinter::~Sprinter()
{

}

void Sprinter::Move(World &world)
{

}