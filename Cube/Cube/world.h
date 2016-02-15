#ifndef WORLD_H__
#define WORLD_H__

#include "array2d.h"
#include "chunk.h"
#include "tool.h"
#include "shader.h"
#include "bloodMoon.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include "simplexnoise.h"



class Player;
class Animal;
class Monster;
class Bear;
class Cow;
class Creeper;

class World
{
public:
	World();
	~World();

	BlockType BlockAt(float x, float y, float z);
	Chunk* ChunkAt(float x, float z);

	Cow* GetCow(int pos) const;
	Bear* GetBear(int pos) const;
	Creeper* GetCreeper(int pos) const;
	Player* GetPlayer() const;

	BloodMoon* GetBloodMoonInstance();
	void SetBloodMoon(BloodMoon* bloodMoon);

	void InitMap(int seed = 0);
	void LoadMap(std::string filename, BlockInfo* &binfo);
	void SaveMap(std::string filename);
	void InitChunks(int CenterX, int CenterZ);
	void Update(int CenterX, int CenterZ, BlockInfo* &info);
	int ChunkNotUpdated(int CenterX, int CenterZ);
	void Render(int CenterX, int CenterZ, GLenum &program);
	void SetUpdateDistance(int updateDist);
	void SpawnMonsters();
	void SpawnCows();
	void SpawnBears();
	void RunWater();
	void RemoveWater(Vector3<float> vf);
	void RemoveLava(Vector3<float> vf);
	
 
private:
	void AddMineral(BlockType mineral, Chunk * &chunk, int x, int y, int z);
	void AddTree(Chunk * &chunk, int x, int y, int z);
	void InitChunk(float i, float j);
	std::thread m_threadChunks;
public:
	bool m_threadcontinue;
	BlockInfo* m_bInfo;
private:
	bool m_started;

	BloodMoon* m_bloodMoon;


	Array2d<Chunk> m_chunks;

	Player* m_player;
	Creeper* m_creeper;
	Cow* m_cow;
	Bear* m_bear;


	int UpdateDistance;
	int	m_seed;

};
#endif // !WORLD_H__


