#include "Inventory.h"



Inventory::Inventory()
{
	for(int i = 0; i < INVENTORY_SIZE; ++i)
	{
		m_objets[i] = Item();
		if(i < FAST_INVENTORY_SIZE)
			m_objetsFast[i] = nullptr;
	}
}
Inventory::~Inventory()
{
	for(int i = 0; i < FAST_INVENTORY_SIZE; ++i)
		if(m_objetsFast[i] != nullptr)
		{
			delete m_objetsFast[i];
			m_objetsFast[i] = nullptr;
		}
}

void Inventory::SwitchItems(int index_item1, int index_item2)
{
}
