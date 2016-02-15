#ifndef ENGINE_H__
#define ENGINE_H__
#include "define.h"
#include "tool.h"
#include "texture.h"
#include "openglcontext.h"
#include "textureatlas.h"
#include "player.h"
#include "shader.h"
#include "chunk.h"
#include "blockinfo.h"
#include "array2d.h"
#include "vector3.h"
#include "world.h"
#include "monster.h"
#include "model3d.h"
#include "animal.h"
#include "vector3.h"
#include "gun.h"
#include "bloodMoon.h"
#include "sound.h"
#include "parametre.h"
#include "menu.h"
#include "network.h"


#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <thread>
#include "parametre.h"
#include "menu.h"
#include "music.h"


class Engine : public OpenglContext
{
public:
	Engine();
	virtual ~Engine();
	virtual void Init();
	virtual void DeInit();
	virtual void LoadResource();
	virtual void UnloadResource();
	virtual void Render(float elapsedTime);
	virtual void KeyPressEvent(unsigned char key);
	virtual void KeyReleaseEvent(unsigned char key);
	virtual void MouseMoveEvent(int x, int y);
	virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
	virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);
	void GetBlocAtCursor();

private:
	void UpdateEnvironement(float gameTime);
	void SyncWithServer();
	void RenderFastInventory() const;
	bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);
	void DrawEnvironement(float gameTime);
	void DrawHud() const;
	void DrawHurtEffect() const;
	void DrawFocusedBlock() const;
	void DrawSky(float gameTime) const;
	void DrawDeathScreen() const;
	void PrintText(unsigned int x, unsigned int y, float size, const std::string & t) const;
	void DrawCross(float r, float g, float b) const;
	void AddTextureToAtlas(BlockType type, const std::string &name, const std::string &path, float hauteur);
	void DrawMenuPrincipal() const;;
	void DrawHitMarker() const;
	void DrawMenuSettings() const;
	void DrawMenuControls() const;
	void DrawMenuButton(int menuItem, std::string text, int xPos, int yPos) const;
	void ManageAllMenuKeys(unsigned char key);
	void ManageMenuEnterKeyPress();
	void DrawMenuSettingSelected(bool isFloat);
	void DrawMenuControlSelected();
	void DrawScope();

	void CloseGame();
	void SetDayOrNight(float gametime);
private:
	bool m_wireframe;
	float m_LastTickTime;
	int m_cptTick = 0;
	float m_LastTickTimeWater;

	GLfloat m_nightLightAmb[4] = { 5.f, 4.f, 3.f, 7.f };
	GLfloat m_dayLightAmb[4] = { 5.f, 5.f, 5.f, 7.f };
	GLfloat m_nightFogColors[4] = { 0.1f, 0.1f, 0.14f, 1 };
	GLfloat m_dayFogColors[4] = { 0.74f, 0.76f, 0.73f, 1 };

	int m_activeMonsters;
	int m_activeAnimals;

	bool m_keyboard[sf::Keyboard::KeyCount]; //tableau de toutes les touches du clavier
	bool m_mouseButton[sf::Mouse::ButtonCount]; //tableau de toutes les touches du clavier

	TextureAtlas m_textureAtlas;
	Texture m_textureSky;
	Texture m_textureFont;
	Texture m_effectHurt;
	Texture m_hitMarker;
	Texture m_textureScope;

	int m_fastInventoryKeySelected;
	
	Shader m_shader01;
	World m_world;

	//Indexe de la texutre dans l'atlas
	TextureAtlas::TextureIndex m_texBlockIndex;
	BlockInfo* m_bInfo;

	//Block qui a le focus
	Vector3<int> m_currentBlock;
	Vector3<int> m_currentFaceNormal;

	//Hui
	int m_fps;
	int m_chunkToUpdate;
	bool displayInfo;
	Music& m_music = Music::GetInstance();



	//Model
	Model3d m_modelCow;
	Model3d m_modelCreeper;
	Model3d m_modelBear;
	Model3d m_modelChicken;

	Network m_network;

	//Gun 
	Gun * playerGun;

	// Parametres
	Parametre& m_settings = Parametre::GetInstance();

	// Valeurs pour le jour et la nuit
	float m_redLight;
	float m_greenLight;
	float m_blueLight;
	float m_redFog;
	float m_greenFog;
	float m_blueFog;
	float m_fogDensity;
	float m_fogStart;

	int m_missingTime;

	bool m_isMenuOpen;
	Menu* m_menu;

	bool m_isInventoryOpen;
};

#endif // ENGINE_H__
