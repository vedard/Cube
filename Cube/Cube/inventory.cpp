#include "inventory.h"


Inventory::Inventory()
{
	for (size_t i = 0; i < sizeof(m_objetsFast); i++)
		m_objetsFast[i] = &m_objets[i];
}


Inventory::~Inventory()
{
	for (size_t i = 0; i < sizeof(m_objetsFast); i++)
		m_objetsFast[i] = new Item;

	delete[] * m_objetsFast;								//freed memory

	for (size_t i = 0; i < sizeof(m_objetsFast); i++)	//pointed dangling ptr to NULL
		m_objetsFast[i] = NULL;
}

void Inventory::AddItemQ(BlockType type)
{
	this->AddItemQ(type, 1);
}

void Inventory::AddItemQ(BlockType type, int number)
{
	bool isAdded = false;

	for each (Item var in m_objets)
	{
		if (var.GetType() == type)
		{
			var.Add(number);
			isAdded = true;
			break;
		}
	}

	if (!isAdded)
	{
		for each (Item var in m_objets)
		{
			if (var.GetQuantity() == 0)
			{
				var.AddNew(type, number);
				break;
			}
		}
	}
}

bool Inventory::RemoveItemQ(BlockType type)
{
	this->RemoveItemQ(type, 1);
}

bool Inventory::RemoveItemQ(BlockType type, int number)
{
	for each (Item var in m_objets)
	{
		if (var.GetType() == type && var.GetQuantity() > 0)
		{
			var.Remove(number);
			break;
		}
	}
}

void Inventory::ReassignItemShortcut(int index_itemFast, int index_item)
{
	if (!(index_itemFast == index_item))
		m_objetsFast[index_itemFast] = &m_objets[index_item];
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

Item * Inventory::GetItems()
{
	return m_objets;
}
