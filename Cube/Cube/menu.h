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
	int m_digitCount = 0; // Compte le nombre de chiffre ins�r�s dans m_settingNewValue pour pouvoir le transf�rer en float
	std::string m_controlSelected;
private:
	int m_menuItemsAmount;
	bool m_keyboard[sf::Keyboard::KeyCount]; //tableau de toutes les touches du clavier
	Parametre& m_settings = Parametre::GetInstance();
};
// POUR AJOUTER UN SETTING...
// Ajouter un setting dans la classe parametre (et toutes ses m�thodes)
// Ajouter une valeur dans l'�num du sous-menu d�sir� dans define.h
// Ajuster le nombre d'�l�ments menu dans le constructeur menu.cpp
// Ajouter les boutons dans DrawMenuSettings/Controls de engine.cpp
// Ajuster la m�thode OnKeyDown de menu.cpp aux fl�ches droite et gauche pour la navigation si elle n'est plus correcte
// Modifier la m�thode ManageMenuEnterKeyPress de engine.cpp
// Modifier la m�thode OnKeyDown de  menu.cpp si un nouveau controle a �t� ajout�

#endif
