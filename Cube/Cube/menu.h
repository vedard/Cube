#ifndef MENU_H__
#define MENU_H__

#include "define.h"
#include "parametre.h"

class Menu
{
public:
	Menu(SOUS_MENU currentMenu);
	~Menu();
	void OnKeyDown(unsigned char key);
	void SaveChanges();
	int m_currentMenuItem;
	SOUS_MENU m_currentMenu;
	int m_settingNewValue = 0;
	int m_digitCount = 0; // Compte le nombre de chiffre insérés dans m_settingNewValue pour pouvoir le transférer en float
	std::string m_controlSelected;
private:
	int m_menuItemsAmount;
	bool m_keyboard[sf::Keyboard::KeyCount]; //tableau de toutes les touches du clavier
	Parametre& m_settings = Parametre::GetInstance();
};

#endif
