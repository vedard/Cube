#ifndef WORLD_H__
#define WORLD_H__

#include "array2d.h"
#include "chunk.h"

class World
{
public:
	World();
	~World();

	BlockType BlockAt(float x, float y, float z, BlockType defaultBlockType);
	Chunk& ChunkAt(float x, float z);


private:
	Array2d<Chunk> m_chunks;
};
#endif // !WORLD_H__


