#include "item.h"

Item::Item()
{
	m_quantity = 0;
	m_type = BTYPE_AIR;
}

Item::Item(BlockType type)
{
	m_quantity = 1;
	m_type = type;
}

Item::Item(BlockType type, int number)
{
	m_quantity = number;
	m_type = type;
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
	return m_type;
}

void Item::Add()
{
	this->Add(1);
}

void Item::Add(int number)
{
	m_quantity += number;
}

void Item::AddNew(BlockType type, int number)
{
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
