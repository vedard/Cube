#include <iostream>
#include "parametre.h"
#include "array2d.h"
#include <string>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

Parametre Parametre::m_instance = Parametre();

Parametre::Parametre()
{
	Load();
}

Parametre::~Parametre()
{
}

Parametre& Parametre::GetInstance()
{
	return m_instance;
}

void Parametre::Save()
{
	ofstream myfile;
	myfile.open("Cube.conf");

	std::cout << "Saving changes on config file" << std::endl;

	
	myfile << "fullscreen " << ((m_isfullscreen) ? "true" : "false") << "\n";
	myfile << "width " << m_width << "\n";
	myfile << "height " << m_height << "\n";
	myfile << "antialiasing " << m_antialiasing << "\n";
	myfile << "vsync " << ((m_vsync) ? "true" : "false") << "\n";
	myfile << "render_distance " << m_renderdistance << "\n";
	myfile << "cross_color_r " << m_crossred << "\n";
	myfile << "cross_color_g " << m_crossgreen << "\n";
	myfile << "cross_color_b " << m_crossblue << "\n";
	myfile << "mouse_sensibility " << m_mousesensibility << "\n";
	myfile << "btnavancer " << m_avancer << "\n";
	myfile << "btngauche " << m_gauche << "\n";
	myfile << "btnreculer " << m_reculer << "\n";
	myfile << "btndroite " << m_droite << "\n";
	myfile << "btnfullscreen " << m_fullscreen << "\n";
	myfile << "btninfo " << m_info << "\n";
	myfile << "btncrouch " << m_crouch << "\n";
	myfile << "btnrun " << m_run << "\n";
	myfile << "btnjump " << m_jump << "\n";
	myfile << "btnnoclip " << m_noclip << "\n";
	myfile << "btninventory1 " << m_inventory1 << "\n";
	myfile << "btninventory2 " << m_inventory2 << "\n";
	myfile << "btninventory3 " << m_inventory3 << "\n";
	myfile << "btninventory4 " << m_inventory4 << "\n";
	myfile << "btninventory " << m_openinventory << "\n";
	myfile << "btnspawnmonster " << m_spawnmonster << "\n";
	myfile << "btnwireframe " << m_wireframe << "\n";
	myfile << "server " << ((m_isServer) ? "true" : "false") << "\n";
	myfile << "musicvolume " << m_musicvolume << "\n";
	myfile << "soundvolume " << m_soundvolume << "\n";
	myfile << "inventaire_rapide1 " << m_inventaire_rapide1 << "\n";
	myfile << "inventaire_rapide2 " << m_inventaire_rapide2 << "\n";
	myfile << "inventaire_rapide3 " << m_inventaire_rapide3 << "\n";
	myfile << "inventaire_bouger_element " << m_inventaire_bouger_element << "\n";
	myfile << "inventaire_enlever_element " << m_inventaire_enlever_element << "\n";
	myfile << "inventaire_creatif " << m_inventaire_creatif << "\n";
	myfile << "playername " << m_PlayerName << "\n";
	myfile << "last_server " << m_lastServer << "\n";
	myfile << "isConnected " << m_isConnected << "\n";

	myfile.close();
	Load();
}

void Parametre::SaveDefault()
{
	ofstream myfile;
	myfile.open("Cube.conf");
	std::cout << "Creating config save file" << std::endl;

	
	myfile << "fullscreen false \n";
	myfile << "width 1360 \n";
	myfile << "height 768 \n";
	myfile << "antialiasing 0 \n";
	myfile << "vsync false \n";
	myfile << "render_distance false \n";
	myfile << "cross_color_r 0 \n";
	myfile << "cross_color_g 0.9 \n";
	myfile << "cross_color_b 0 \n";
	myfile << "mouse_sensibility 0.1 \n";
	myfile << "btnavancer 22 \n";
	myfile << "btngauche 0 \n";
	myfile << "btnreculer 18 \n";
	myfile << "btndroite 3 \n";
	myfile << "btnfullscreen 94 \n";
	myfile << "btninfo 87 \n";
	myfile << "btncrouch 37 \n";
	myfile << "btnrun 38 \n";
	myfile << "btnjump 57 \n";
	myfile << "btnnoclip 21 \n";
	myfile << "btninventory1 27 \n";
	myfile << "btninventory2 28 \n";
	myfile << "btninventory3 29 \n";
	myfile << "btninventory4 30 \n";
	myfile << "btninventory 8 \n";
	myfile << "btnspawnmonster 12 \n";
	myfile << "btnwireframe 24 \n";
	myfile << "server" << " false" << "\n";
	myfile << "musicvolume 10 \n";
	myfile << "soundvolume 12 \n";
	myfile << "inventaire_rapide1 " << FIRST_FAST_INVENTORY_KEY << "\n";
	myfile << "inventaire_rapide2 " << SECOND_FAST_INVENTORY_KEY << "\n";
	myfile << "inventaire_rapide3 " << THIRD_FAST_INVENTORY_KEY << "\n";
	myfile << "inventaire_bouger_element " << INVENTORY_MOVE_ITEM_KEY << "\n";
	myfile << "inventaire_enlever_element " << INVENTORY_DELETE_ITEM_KEY << "\n";
	myfile << "inventaire_creatif " << IS_INVENTORY_CREATIVE << "\n";
	myfile << "playername Player1 \n";
	myfile << "last_server none \n";


	myfile.close();
	Load();

}

void Parametre::Load()
{
	const std::string& filename = "Cube.conf";

	std::cout << "Checking if " << filename << " exist" << std::endl;

	//Open file


	std::ifstream file(filename);
	if (!file.good())
	{
		std::cout << "No config file found" << std::endl;

		Save();
	}
	else
	{
		Array2d<std::string> setting(100, 2);
		setting.Reset("null");
		setting.Get(0, 0) = "width";
		setting.Get(1, 0) = "height";
		setting.Get(2, 0) = "antialiasing";
		setting.Get(3, 0) = "fullscreen";
		setting.Get(4, 0) = "vsync";
		setting.Get(5, 0) = "render_distance";
		setting.Get(6, 0) = "cross_color_r";
		setting.Get(7, 0) = "cross_color_g";
		setting.Get(8, 0) = "cross_color_b";
		setting.Get(9, 0) = "mouse_sensibility";
		setting.Get(10, 0) = "btnavancer";
		setting.Get(11, 0) = "btngauche";
		setting.Get(12, 0) = "btnreculer";
		setting.Get(13, 0) = "btndroite";
		setting.Get(14, 0) = "btnfullscreen";
		setting.Get(15, 0) = "btninfo";
		setting.Get(16, 0) = "btncrouch";
		setting.Get(17, 0) = "btnrun";
		setting.Get(18, 0) = "btnjump";
		setting.Get(19, 0) = "btnnoclip";
		setting.Get(20, 0) = "btninventory1";
		setting.Get(21, 0) = "btninventory2";
		setting.Get(22, 0) = "btninventory3";
		setting.Get(23, 0) = "btninventory4";
		setting.Get(24, 0) = "btninventory";
		setting.Get(25, 0) = "btnspawnmonster";
		setting.Get(26, 0) = "btnwireframe";
		setting.Get(27, 0) = "server";
		setting.Get(28, 0) = "musicvolume";
		setting.Get(29, 0) = "soundvolume";
		setting.Get(30, 0) = "inventaire_rapide1";
		setting.Get(31, 0) = "inventaire_rapide2";
		setting.Get(32, 0) = "inventaire_rapide3";
		setting.Get(33, 0) = "inventaire_bouger_element";
		setting.Get(34, 0) = "inventaire_enlever_element";
		setting.Get(35, 0) = "inventaire_creatif";
		setting.Get(36, 0) = "playername";
		setting.Get(37, 0) = "last_server";

		file.open(filename);
		std::cout << "Reading " << filename << "..." << std::endl;

		//Read whole file
		std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		std::stringstream ss(str);
		//Close it
		file.close();

		//read setting value
		int cpt = 0;
		while (ss)
		{
			std::string temp;
			cpt++;
			ss >> temp;
			int index = setting.GetIndex(temp);
			if (index != -1)
				ss >> setting.Get(setting.GetIndex(temp), 1);
		}
		if (cpt <= 36)
		{
			cout << "File incomplete, recreating";
			SaveDefault();
		}
		//Convert string into int
		if (setting.Get(0, 1) != "null")
			m_width = atoi(setting.Get(0, 1).c_str());

		if (setting.Get(1, 1) != "null")
			m_height = atoi(setting.Get(1, 1).c_str());

		if (setting.Get(2, 1) != "null")
			m_antialiasing = atoi(setting.Get(2, 1).c_str());

		if (setting.Get(3, 1) == "true" || setting.Get(3, 1) == "1")
			m_isfullscreen = true;
		else
			m_isfullscreen = false;

		if (setting.Get(4, 1) == "true" || setting.Get(4, 1) == "1")
			m_vsync = true;
		else
			m_vsync = false;

		if (setting.Get(5, 1) != "null")
			m_renderdistance = atoi(setting.Get(5, 1).c_str());
		if (setting.Get(6, 1) != "null")
			m_crossred = (float)atof(setting.Get(6, 1).c_str());
		if (setting.Get(7, 1) != "null")
			m_crossgreen = (float)atof(setting.Get(7, 1).c_str());
		if (setting.Get(8, 1) != "null")
			m_crossblue = (float)atof(setting.Get(8, 1).c_str());
		if (setting.Get(9, 1) != "null")
			m_mousesensibility = (float)atof(setting.Get(9, 1).c_str());
		if (setting.Get(10, 1) != "null")
			m_avancer = (sf::Keyboard::Key)atoi(setting.Get(10, 1).c_str());
		if (setting.Get(11, 1) != "null")
			m_gauche = (sf::Keyboard::Key)atoi(setting.Get(11, 1).c_str());
		if (setting.Get(12, 1) != "null")
			m_reculer = (sf::Keyboard::Key)atoi(setting.Get(12, 1).c_str());
		if (setting.Get(13, 1) != "null")
			m_droite = (sf::Keyboard::Key)atoi(setting.Get(13, 1).c_str());
		if (setting.Get(14, 1) != "null")
			m_fullscreen = (sf::Keyboard::Key)atoi(setting.Get(14, 1).c_str());
		if (setting.Get(15, 1) != "null")
			m_info = (sf::Keyboard::Key)atoi(setting.Get(15, 1).c_str());
		if (setting.Get(16, 1) != "null")
			m_crouch = (sf::Keyboard::Key)atoi(setting.Get(16, 1).c_str());
		if (setting.Get(17, 1) != "null")
			m_run = (sf::Keyboard::Key)atoi(setting.Get(17, 1).c_str());
		if (setting.Get(18, 1) != "null")
			m_jump = (sf::Keyboard::Key)atoi(setting.Get(18, 1).c_str());
		if (setting.Get(19, 1) != "null")
			m_noclip = (sf::Keyboard::Key)atoi(setting.Get(19, 1).c_str());
		if (setting.Get(20, 1) != "null")
			m_inventory1 = (sf::Keyboard::Key)atoi(setting.Get(20, 1).c_str());
		if (setting.Get(21, 1) != "null")
			m_inventory2 = (sf::Keyboard::Key)atoi(setting.Get(21, 1).c_str());
		if (setting.Get(22, 1) != "null")
			m_inventory3 = (sf::Keyboard::Key)atoi(setting.Get(22, 1).c_str());
		if (setting.Get(23, 1) != "null")
			m_inventory4 = (sf::Keyboard::Key)atoi(setting.Get(23, 1).c_str());
		if (setting.Get(24, 1) != "null")
			m_openinventory = (sf::Keyboard::Key)atoi(setting.Get(24, 1).c_str());
		if (setting.Get(25, 1) != "null")
			m_spawnmonster = (sf::Keyboard::Key)atoi(setting.Get(25, 1).c_str());
		if (setting.Get(26, 1) != "null")
			m_wireframe = (sf::Keyboard::Key)atoi(setting.Get(26, 1).c_str());
		if (setting.Get(27, 1) == "true" || setting.Get(27, 1) == "1")
			m_isServer = true;
		else
			m_isServer = false;
		if (setting.Get(28, 1) != "null")
			m_musicvolume = atoi(setting.Get(28, 1).c_str());
		if (setting.Get(29, 1) != "null")
			m_soundvolume = atoi(setting.Get(29, 1).c_str());
		if (setting.Get(30, 1) != "null")
			m_inventaire_rapide1 = (sf::Keyboard::Key)atoi(setting.Get(30, 1).c_str());
		if (setting.Get(31, 1) != "null")
			m_inventaire_rapide2 = (sf::Keyboard::Key)atoi(setting.Get(31, 1).c_str());
		if (setting.Get(32, 1) != "null")
			m_inventaire_rapide3 = (sf::Keyboard::Key)atoi(setting.Get(32, 1).c_str());
		if (setting.Get(33, 1) != "null")
			m_inventaire_bouger_element = (sf::Keyboard::Key)atoi(setting.Get(33, 1).c_str());
		if (setting.Get(34, 1) != "null")
			m_inventaire_enlever_element = (sf::Keyboard::Key)atoi(setting.Get(34, 1).c_str());
		m_inventaire_creatif = (setting.Get(35, 1) == "true" || setting.Get(35,1) == "1");
		if (setting.Get(36, 1) != "null")
			m_PlayerName = setting.Get(36, 1);
		if (setting.Get(37, 1) != "null")
			m_lastServer = setting.Get(37, 1);
	}
}
