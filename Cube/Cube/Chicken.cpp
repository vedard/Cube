#include "chicken.h"

Chicken::Chicken() :Animal(A_CHICKEN)
{
	m_health = 50;
	m_dimension = Vector3<float>(0.75f, 0.75f, 0.75f);
	m_Name = "Chicken";
}

Chicken::~Chicken()
{
}
