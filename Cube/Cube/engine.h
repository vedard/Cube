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
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>


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

	template <class T>
	BlockType BlockAt(T x, T y, T z, BlockType defaultBlockType)
	{
		Vector3<float>chunkPos(floor(x / CHUNK_SIZE_X), 0, floor(z / CHUNK_SIZE_Z));

		if (chunkPos.x >= 0 && chunkPos.z >= 0 && chunkPos.x < WORLD_SIZE && chunkPos.z < WORLD_SIZE)
		{

			Vector3<float>blockPos(x - (chunkPos.x * CHUNK_SIZE_X), y, z - (chunkPos.z * CHUNK_SIZE_X));

			return m_Chunks.Get(chunkPos.x, chunkPos.z).GetBlock(blockPos.x , blockPos.y, blockPos.z );
		}
		else
			return defaultBlockType;
	}



private:
	bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);
	void DrawHud();
	void PrintText(unsigned int x, unsigned int y, int size, const std::string & t);

private:
	bool m_wireframe;

	Player m_player;

	bool m_keyboard[sf::Keyboard::KeyCount]; //tableau de toutes les touches du clavier

	TextureAtlas m_textureAtlas;
	Texture m_textureSky;
	Texture m_textureFont;
	Texture m_textureCrosshair;
	Shader m_shader01;
	Array2d<Chunk> m_Chunks;

	//Indexe de la texutre dans l'atlas
	TextureAtlas::TextureIndex m_texBlockIndex;
	BlockInfo* m_bInfo;

	//Block qui a le focus
	Vector3<float> m_currentBlock;
	Vector3<float> m_currentFaceNormal;


	int m_fps;


};

#endif // ENGINE_H__
