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
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <thread>


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
	bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);
	void DrawHud() const;
	void PrintText(unsigned int x, unsigned int y, float size, const std::string & t) const;
	void DrawCross(float r, float g, float b) const;
	void Play(sf::SoundBuffer &soundBuffer, int volume = 15);

private:
	bool m_wireframe;

	Player m_player;
	Monster* m_monster;
	
	bool m_keyboard[sf::Keyboard::KeyCount]; //tableau de toutes les touches du clavier

	TextureAtlas m_textureAtlas;
	Texture m_textureSky;
	Texture m_textureFont;
	Texture* m_textureGun;
	
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

	sf::SoundBuffer m_SoundGunShot;
	sf::SoundBuffer m_SoundGunShot2;
	sf::SoundBuffer m_SoundGunDraw;

	sf::SoundBuffer m_SoundFleshImpact;
	
	sf::SoundBuffer * m_SoundStep;

	sf::Sound* sound;


};

#endif // ENGINE_H__
