#include "menu.h"

Menu::Menu(SOUS_MENU currentMenu) : m_currentMenu(currentMenu)
{
	m_currentMenuItem = 0;

	if (m_currentMenu == SM_PRINCIPAL)
		m_menuItemsAmount = 2; // Starts at 0
	else if (m_currentMenu == SM_SETTINGS)
		m_menuItemsAmount = 9;
	else if (m_currentMenu == SM_CONTROLS)
		m_menuItemsAmount = 16;

	//Initialisation des touches
	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
		m_keyboard[i] = false;
}

Menu::~Menu()
{

}

void Menu::OnKeyDown(unsigned char key)
{
	m_keyboard[key] = true;

	// Quand on modifie un settings dans le menu par clavier...
	if (m_currentMenu == SM_SETTING_SELECTED)
	{
		if (m_keyboard[sf::Keyboard::BackSpace])
		{
			if (m_settingNewValue > 0)
			{
				m_digitCount--;
				m_settingNewValue = m_settingNewValue / 10;
			}
		}
		else
		{
			m_digitCount++;

			int value;
			if (m_keyboard[sf::Keyboard::Num0])
				value = 0;
			else if (m_keyboard[sf::Keyboard::Num1])
				value = 1;
			else if (m_keyboard[sf::Keyboard::Num2])
				value = 2;
			else if (m_keyboard[sf::Keyboard::Num3])
				value = 3;
			else if (m_keyboard[sf::Keyboard::Num4])
				value = 4;
			else if (m_keyboard[sf::Keyboard::Num5])
				value = 5;
			else if (m_keyboard[sf::Keyboard::Num6])
				value = 6;
			else if (m_keyboard[sf::Keyboard::Num7])
				value = 7;
			else if (m_keyboard[sf::Keyboard::Num8])
				value = 8;
			else if (m_keyboard[sf::Keyboard::Num9])
				value = 9;
			else // Si ce n'est pas un chiffre qui a été cliqué...
				m_digitCount--;

			m_settingNewValue = m_settingNewValue * 10 + value;
		}
	}
	else // Quand on navigue dans les menus
	{
		if (m_keyboard[sf::Keyboard::Up])
		{
			m_currentMenuItem--;

			if (m_currentMenuItem <= -1)
				m_currentMenuItem = m_menuItemsAmount;
		}
		else if (m_keyboard[sf::Keyboard::Left])
		{
			if (m_currentMenu == SM_SETTINGS)
			{
				m_currentMenuItem -= ((m_menuItemsAmount + 1) / 2); // +1 parce que ça commence à zéro

				if (m_currentMenuItem < 0)
					m_currentMenuItem += (m_menuItemsAmount + 1);
			}
			else if (m_currentMenu == SM_CONTROLS)
			{
				if (m_currentMenuItem < 2) // Nombre d'items dans la derniere colonne
					m_currentMenuItem += 3; // Nombre d'items par colonne - nombre d'items dans la derniere colonne
				else if (m_currentMenuItem < 5) // Nombre d'items par colonne
					m_currentMenuItem -= 2;

				m_currentMenuItem -= 5; // Nombre d'items par colonne

				if (m_currentMenuItem < 0)
					m_currentMenuItem += (m_menuItemsAmount + 1);
			}
		}
		else if (m_keyboard[sf::Keyboard::Right])
		{
			if (m_currentMenu == SM_SETTINGS)
			{
				m_currentMenuItem += ((m_menuItemsAmount + 1) / 2); // +1 parce que ça commence à zéro

				if (m_currentMenuItem > m_menuItemsAmount)
					m_currentMenuItem -= (m_menuItemsAmount + 1);
			}
			else if (m_currentMenu == SM_CONTROLS)
			{
				if (m_currentMenuItem >= 15)
					m_currentMenuItem -= 3; // Nombre d'items par colonne - nombre d'items dans la derniere colonne
				else if (m_currentMenuItem >= 12)
					m_currentMenuItem += 2;

				m_currentMenuItem += 5; // Nombre d'items par colonne

				if (m_currentMenuItem > m_menuItemsAmount)
					m_currentMenuItem -= (m_menuItemsAmount + 1);
			}
		}
		else if (m_keyboard[sf::Keyboard::Down])
		{
			m_currentMenuItem++;

			if (m_currentMenuItem > m_menuItemsAmount)
				m_currentMenuItem = 0;
		}
	}

	m_keyboard[key] = false;
}

void Menu::SaveChanges()
{
	
}