#ifndef WORLD_H__
#define WORLD_H__

#include "array2d.h"
#include "chunk.h"
#include "perlin.h"

class World
{
public:
	World();
	~World();

	BlockType BlockAt(float x, float y, float z);
	Chunk& ChunkAt(float x, float z);

	void LoadFlatMap();


private:
	Array2d<Chunk> m_chunks;
	Perlin perlin;
};
#endif // !WORLD_H__


