#include "bear.h"


Bear::Bear() :Animal(A_BEAR)
{
	m_health = 200;
	m_dimension = Vector3<float>(1.5f, 2.3f, 2.4f);
	m_Name = "BigBadBear aka BBB";
};
Bear::~Bear()
{
};




