#ifndef ITEM_H__
#define ITEM_H__

#include <string>
#include "define.h"

class Item
{
public:
	Item();
	~Item();

	int GetQuantity();
	virtual BlockType GetType() const;

	void Add();
	void Add(int number);

	void Remove();
	void Remove(int number);

private:
	int m_quantity;
	bool m_isselected;
	bool m_isfast;

};

#endif //ITEM_H__