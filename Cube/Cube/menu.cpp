#include "menu.h"

Menu::Menu(SOUS_MENU currentMenu) : m_currentMenu(currentMenu)
{
	m_currentMenuItem = 0;

	if (m_currentMenu == SM_PRINCIPAL)
		m_menuItemsAmount = 2; // Starts at 0

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
		
		m_keyboard[key] = false;
	}
	else if (m_keyboard[sf::Keyboard::Return])
	{
		
	}
	else if (m_keyboard[sf::Keyboard::Left])
	{

	}
	else if (m_keyboard[sf::Keyboard::Right])
	{

	}
	else if (m_keyboard[sf::Keyboard::Escape])
	{

	}
	else if (m_keyboard[sf::Keyboard::Down])
	{
		m_currentMenuItem++;

		if (m_currentMenuItem > m_menuItemsAmount)
			m_currentMenuItem = 0;

		m_keyboard[key] = false;
	}
 }
