#include "sprinter.h"

Sprinter::Sprinter() :Monster(M_SPRINTER)
{
	m_health = 100000;
	m_AttackSpeed = .5f;
	//m_dimension = Vector3<float>(3.f, 4.f, 0.7f);
}
Sprinter::~Sprinter()
{

}

//void Sprinter::Move(World &world)
//{
//	
//}