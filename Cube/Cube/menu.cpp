#include "menu.h"

Menu::Menu(SOUS_MENU currentMenu) : m_currentMenu(currentMenu)
{
	m_currentMenuItem = 0;

	if (m_currentMenu == SM_PRINCIPAL)
		m_menuItemsAmount = 2; // Starts at 0
	else if (m_currentMenu == SM_SETTINGS)
		m_menuItemsAmount = 11;
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
	else if (m_currentMenu == SM_CONTROL_SELECTED)
	{
		// Vérifier si la touche est déjà utilisée
		if (m_keyboard[m_settings.m_avancer] || m_keyboard[m_settings.m_gauche] ||
			m_keyboard[m_settings.m_reculer] || m_keyboard[m_settings.m_droite] ||
			m_keyboard[m_settings.m_fullscreen] || m_keyboard[m_settings.m_info] ||
			m_keyboard[m_settings.m_crouch] || m_keyboard[m_settings.m_run] ||
			m_keyboard[m_settings.m_jump] || m_keyboard[m_settings.m_noclip] ||
			m_keyboard[m_settings.m_inventory1] || m_keyboard[m_settings.m_inventory2] ||
			m_keyboard[m_settings.m_inventory3] || m_keyboard[m_settings.m_inventory4] ||
			m_keyboard[m_settings.m_openinventory] || m_keyboard[m_settings.m_spawnmonster] ||
			m_keyboard[m_settings.m_wireframe])
			m_controlSelected = KEY_ALREADY_BOUND;
		else
		{
			if (m_currentMenuItem == MC_AVANCER)
				m_settings.m_avancer = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_GAUCHE)
				m_settings.m_gauche = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_RECULER)
				m_settings.m_reculer = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_DROITE)
				m_settings.m_droite = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_FULLSCREEN)
				m_settings.m_fullscreen = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_INFO)
				m_settings.m_info = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_CROUCH)
				m_settings.m_crouch = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_RUN)
				m_settings.m_run = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_JUMP)
				m_settings.m_jump = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_NOCLIP)
				m_settings.m_noclip = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_INVENTORY1)
				m_settings.m_inventory1 = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_INVENTORY2)
				m_settings.m_inventory2 = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_INVENTORY3)
				m_settings.m_inventory3 = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_INVENTORY4)
				m_settings.m_inventory4 = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_INVENTORY)
				m_settings.m_openinventory = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_SPAWNMONSTER)
				m_settings.m_spawnmonster = (sf::Keyboard::Key)key;
			else if (m_currentMenuItem == MC_WIREFRAME)
				m_settings.m_wireframe = (sf::Keyboard::Key)key;

			m_settings.Save();
			m_controlSelected = KEY_BINDED_SUCCESSFULLY;
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
