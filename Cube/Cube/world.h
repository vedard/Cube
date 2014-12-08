#ifndef WORLD_H__
#define WORLD_H__

#include "array2d.h"
#include "chunk.h"
#include "perlin.h"
#include "tool.h"
#include "shader.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

class World
{
public:
	World();
	~World();

	BlockType BlockAt(float x, float y, float z);
	Chunk& ChunkAt(float x, float z);

	// - Octaves: Irrigulité
	// - Freq: Agressivité des montagnes (0 for flat)
	// - Amp: Hauteur des montagnes
	// - Seed: seed pour le random
	void InitMap(int octaves, float freq, float amp, int seed);

	void LoadMap(std::string filename, BlockInfo *binfo);
	void SaveMap(std::string filename);
	void Update(int CenterX, int CenterZ, BlockInfo* info);

private:
	void AddMineral(BlockType mineral, int i, int j, int x, int y, int z);
	void AddTree(int i, int j, int x, int y, int z);


private:
	Array2d<Chunk> m_chunks;

};
#endif // !WORLD_H__


