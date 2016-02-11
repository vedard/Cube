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
// POUR AJOUTER UN SETTING...
// Ajouter un setting dans la classe parametre (et toutes ses méthodes)
// Ajouter une valeur dans l'énum du sous-menu désiré dans define.h
// Ajuster le nombre d'éléments menu dans le constructeur menu.cpp
// Ajouter les boutons dans DrawMenuSettings/Controls de engine.cpp
// Ajuster la méthode OnKeyDown de menu.cpp aux flèches droite et gauche pour la navigation si elle n'est plus correcte
// Modifier la méthode ManageMenuEnterKeyPress de engine.cpp
// Modifier la méthode OnKeyDown de  menu.cpp si un nouveau controle a été ajouté

#endif
