#ifndef INVENTORY_H__
#define INVENTORY_H__

#include "item.h"
#include "define.h"
#include <iostream>

class Inventory
{
public:
	Inventory();
	~Inventory();

	void AddItemQ(BlockType type);
	void AddItemQ(BlockType type, int number);

	bool RemoveItemQ(BlockType type);
	bool RemoveItemQ(BlockType type, int number);
	bool RemoveItemALL(BlockType type);	//TODO: Remove all of this item from inventory

	void ReassignItemShortcut(int index_itemFast, int index_item);
	void SwitchItems(int index_item1, int index_item2);
	void SortInventory();	//TODO: Sort inventory to eliminate & fuse possible doubled stacks as well as ordering them by BlockType

	Item* GetItems();
private:

	Item m_objets [INVENTORY_SIZE];
	Item *m_objetsFast[FAST_INVENTORY_SIZE];
};

#endif //INVENTORY_H__