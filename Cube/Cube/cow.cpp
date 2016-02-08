#include "cow.h"

Cow::Cow():Animal(A_COW)
{
	m_health = 140;
	m_dimension = Vector3<float>(3.f, 4.6f, 4.8f);
}

Cow::~Cow()
{
}


