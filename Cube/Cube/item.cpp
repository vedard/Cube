#include "item.h"

Item::Item()
{
	m_quantity = 1;
}


Item::~Item()
{
}

int Item::GetQuantity()
{
	return m_quantity;
}

BlockType Item::GetType() const
{
	return BlockType();
}

void Item::Add()
{
	this->Add(1);
}

void Item::Add(int number)
{
	m_quantity += number;
}

void Item::Remove()
{
	this->Remove(1);
}

void Item::Remove(int number)
{
	if (m_quantity > 0)
		m_quantity -= number;
}
