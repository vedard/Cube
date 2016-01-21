#pragma once
#include <string>
class Item
{
public:
	Item();
	~Item();

private:
	std::string m_nom;
	int m_quantity;
	bool m_isselected;
	bool m_isfast;

};

