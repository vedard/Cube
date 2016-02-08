#ifndef ITEM_H__
#define ITEM_H__

#include <string>
#include "define.h"

class Item
{
public:
	Item();
	Item(BlockType type);
	Item(BlockType type, int number);
	~Item();

	int GetQuantity();
	virtual BlockType GetType() const;

	void Add();
	void Add(int number);
	void AddNew(BlockType type, int number);

	void Remove();
	void Remove(int number);

	void Empty();

private:
	int m_quantity;
	BlockType m_type;
	bool m_isselected;
	bool m_isfast;

};

#endif //ITEM_H__