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
	bool RemoveItemALL(BlockType type);
	
	void SwitchItems(int index_item1, int index_item2);

	Item* GetItems();

	void OnKeyDown(unsigned char key);

	int GetCurrentIndex();
	void SetCurrentIndex(int currentIndex);

	int GetCurrentMoveIndex();
	void Init();
	
private:
	int m_currentIndex;
	int m_currentMoveIndex;
	Item m_objets [INVENTORY_SIZE];
};

#endif //INVENTORY_H__
