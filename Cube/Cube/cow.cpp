#include "cow.h"

Cow::Cow():Animal(A_COW)
{
	m_health = 140;
	m_dimension = Vector3<float>(1.5f, 2.3f, 2.4f);
	m_Name = "Cow";
}

Cow::~Cow()
{
}


