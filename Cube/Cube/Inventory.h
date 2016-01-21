#pragma once
#include "item.h"
#include "define.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	void ReassignItemShortcut(int index_itemFast, int index_item);
	void SwitchItems(int index_item1, int index_item2);
private:

	Item m_objets [INVENTORY_SIZE];
	Item *m_objetsFast[FAST_INVENTORY_SIZE];
};