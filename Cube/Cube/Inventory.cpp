#include "Inventory.h"



Inventory::Inventory()
{
	for (size_t i = 0; i < FAST_INVENTORY_SIZE; i++)
	{
		m_objetsFast[i] = &m_objets[i];
	}
}


Inventory::~Inventory()
{
	for (size_t i = 0; i < FAST_INVENTORY_SIZE; i++)
	{
		m_objetsFast[i] = new Item;
	}

	delete[] *m_objetsFast;								//freed memory

	for (size_t i = 0; i < FAST_INVENTORY_SIZE; i++)	//pointed dangling ptr to NULL
	{
		m_objetsFast[i] = NULL;
	}
}

void Inventory::ReassignItemShortcut(int index_itemFast, int index_item)
{
	if (!(index_itemFast == index_item))
	{
		m_objetsFast[index_itemFast] = &m_objets[index_item];
	}
}

void Inventory::SwitchItems(int index_item1, int index_item2)
{
	if (!(index_item1 == index_item2))
	{
		Item tempItem = m_objets[index_item1];
		m_objets[index_item1] = m_objets[index_item2];
		m_objets[index_item2] = tempItem;
	}
}
