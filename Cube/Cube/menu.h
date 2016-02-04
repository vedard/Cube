#ifndef MENU_H__
#define MENU_H__

#include "define.h"

class Menu
{
public:
	Menu(SOUS_MENU currentMenu);
	~Menu();
	void OnKeyDown(unsigned char key);
	void SaveChanges();
	int m_currentMenuItem;
	SOUS_MENU m_currentMenu;
private:
	int m_menuItemsAmount;
	bool m_keyboard[sf::Keyboard::KeyCount]; //tableau de toutes les touches du clavier

	// Settings
	int m_settingi; // Sera utilisé lorsque l'utilisateur changera n'importe quel setting int dans le menu
	float m_settingf;
};

#endif
