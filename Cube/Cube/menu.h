#ifndef MENU_H__
#define MENU_H__

#include "define.h"

class Menu
{
public:
	Menu(SOUS_MENU currentMenu);
	~Menu();
	void OnKeyDown(unsigned char key);
	int m_currentMenuItem;
private:
	SOUS_MENU m_currentMenu;
	int m_menuItemsAmount;
	bool m_keyboard[sf::Keyboard::KeyCount]; //tableau de toutes les touches du clavier
};

#endif MENU_H__