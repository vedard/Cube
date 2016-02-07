#include "menu.h"

Menu::Menu(SOUS_MENU currentMenu) : m_currentMenu(currentMenu)
{
	m_currentMenuItem = 0;

	if (m_currentMenu == SM_PRINCIPAL)
		m_menuItemsAmount = 2; // Starts at 0
	else if (m_currentMenu == SM_SETTINGS)
		m_menuItemsAmount = 9;

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
	}
	else if (m_keyboard[sf::Keyboard::Right])
	{
		if (m_currentMenu == SM_SETTINGS)
		{
			m_currentMenuItem += ((m_menuItemsAmount + 1) / 2); // +1 parce que ça commence à zéro

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
	
	// Quand on modifie un settings dans le menu par clavier...
	if (m_currentMenu == SM_SETTING_SELECTED)
	{
		if (m_keyboard[sf::Keyboard::BackSpace])
			m_settingNewValue = m_settingNewValue / 10;
		else
		{
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

			m_settingNewValue = m_settingNewValue * 10 + value;
		}
	}

	m_keyboard[key] = false;
}

void Menu::SaveChanges()
{
	
}