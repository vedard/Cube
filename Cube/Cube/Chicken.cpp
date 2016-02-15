#include "chicken.h"

Chicken::Chicken() :Animal(A_CHICKEN)
{
	m_health = 50;
	m_dimension = Vector3<float>(1.f, 1.5f, 1.3f);
	m_Name = "Chicken";
}

Chicken::~Chicken()
{
}
