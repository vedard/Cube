#pragma once
#include "item.h"

#define INVENTORY_SIZE 15
#define FAST_INVENTORY_SIZE 3

class Inventory
{
public:
	Inventory();
	~Inventory();

private:
	void SwitchItems(int index_item1, int index_item2);

	Item m_objets [INVENTORY_SIZE];
	Item m_objetsFast[FAST_INVENTORY_SIZE];
};