#ifndef INVENTORY_H__
#define INVENTORY_H__

#include "item.h"
#include "define.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	void AddItemQ(Item type);
	void AddItemQ(Item type, int number);

	void RemoveItemQ(Item type);
	void RemoveItemQ(Item type, int number);

	void ReassignItemShortcut(int index_itemFast, int index_item);
	void SwitchItems(int index_item1, int index_item2);

	Item* GetItems();
private:

	Item m_objets [INVENTORY_SIZE];
	Item *m_objetsFast[FAST_INVENTORY_SIZE];
};

#endif //INVENTORY_H__