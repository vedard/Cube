#ifndef WORLD_H__
#define WORLD_H__

#include "array2d.h"
#include "chunk.h"
#include "tool.h"
#include "shader.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include "simplexnoise.h"

class World
{
public:
	World();
	~World();

	BlockType BlockAt(float x, float y, float z) const;
	Chunk* ChunkAt(float x, float z);

	void InitMap(int seed = 0);

	void LoadMap(std::string filename, BlockInfo* &binfo);
	void SaveMap(std::string filename);
	void Update(int CenterX, int CenterZ,  BlockInfo* &info);
	int ChunkNotUpdated(int CenterX, int CenterZ);
	void Render(int CenterX, int CenterZ, GLenum &m_program);

private:
	void AddMineral(BlockType mineral, int i, int j, int x, int y, int z);
	void AddTree(int i, int j, int x, int y, int z);



private:
	Array2d<Chunk> m_chunks;
	int		m_seed;

};


#endif // !WORLD_H__


