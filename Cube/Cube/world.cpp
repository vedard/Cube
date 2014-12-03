#include "world.h"


World::World() : m_chunks(WORLD_SIZE, WORLD_SIZE)
{
}


World::~World()
{
}

BlockType World::BlockAt(float x, float y, float z, BlockType defaultBlockType)
{
	Vector3<float>chunkPos(floor(x / CHUNK_SIZE_X), 0, floor(z / CHUNK_SIZE_Z));

	if (chunkPos.x >= 0 && chunkPos.z >= 0 && chunkPos.x < WORLD_SIZE && chunkPos.z < WORLD_SIZE)
	{

		Vector3<float>blockPos(x - (chunkPos.x * CHUNK_SIZE_X), y, z - (chunkPos.z * CHUNK_SIZE_X));

		return m_chunks.Get(chunkPos.x, chunkPos.z).GetBlock(blockPos.x, blockPos.y, blockPos.z);
	}
	else
		return defaultBlockType;
}

Chunk& World::ChunkAt(float x, float z)
{
	return m_chunks.Get(x, z);

}