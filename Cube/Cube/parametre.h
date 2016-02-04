#ifndef PARAMETRE_H__
#define PARAMETRE_H__

#include "define.h"

class Parametre
{
public:
	static Parametre& GetInstance();

	//Controles
	sf::Keyboard::Key m_avancer = sf::Keyboard::W; //Avancer
	sf::Keyboard::Key m_gauche = sf::Keyboard::A; //Gauche
	sf::Keyboard::Key m_reculer = sf::Keyboard::S; //Reculer
	sf::Keyboard::Key m_droite = sf::Keyboard::D; // Droite
	sf::Keyboard::Key m_fullscreen = sf::Keyboard::F10; // Mettre en fullscreen
	sf::Keyboard::Key m_menu = sf::Keyboard::Escape; // Ouvrir le menu
	sf::Keyboard::Key m_crouch = sf::Keyboard::LControl; // S'accroupir
	sf::Keyboard::Key m_run = sf::Keyboard::LShift; // Courir
	sf::Keyboard::Key m_jump = sf::Keyboard::Space; //Sauer
	sf::Keyboard::Key m_noclip = sf::Keyboard::V; //Activer le noclip
	sf::Keyboard::Key m_inventory1 = sf::Keyboard::Num1; // Prendre l'item dans le slot 1
	sf::Keyboard::Key m_inventory2 = sf::Keyboard::Num2; //Prendre l'item dans le slot 2
	sf::Keyboard::Key m_inventory3 = sf::Keyboard::Num3; //Prendre l'item dans le slot 3
	sf::Keyboard::Key m_inventory4 = sf::Keyboard::Num4; //Prendre l'item dans le slot 4
	sf::Keyboard::Key m_openinventory = sf::Keyboard::I; // Ouvrir l'inventaire
	sf::Keyboard::Key m_spawnmonster = sf::Keyboard::M; // Faire spawner les monstres
	sf::Keyboard::Key m_wireframe = sf::Keyboard::Y; // Activer le mode wireframe
	sf::Keyboard::Key m_info = sf::Keyboard::F3; // montrer les fps etc...

	// Display
	bool m_isfullscreen = false;
	bool m_isServer = false;
	int m_width = 1600;
	int m_height = 900;
	int m_antialiasing = 0;
	bool m_vsync = false;
	int m_renderdistance = 6;

	//Crosshair

	float m_crossred = 0.0f;
	float m_crossgreen = 0.9f;
	float m_crossblue = 0.0f;

	float m_mousesensibility = 0.1f;

	//Sound
	int m_musicvolume = 10;
	int m_soundvolume = 12;

	void Save();

private:
	Parametre& operator= (const Parametre&) {}
	Parametre(const Parametre&) {}

	static Parametre m_instance;
	Parametre();
	~Parametre();

	void SaveDefault();
	void Load();
};

#endif