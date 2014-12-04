#include "world.h"


World::World() : m_chunks(WORLD_SIZE, WORLD_SIZE)
{
	//Parcours les chunks et les positionne dans la map
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
			m_chunks.Get(i, j).SetPosition(CHUNK_SIZE_X * i, 0, CHUNK_SIZE_Z * j);
}


World::~World()
{
}

void World::LoadFlatMap()
{
	//Parcous tous les bloack pour les placer et creer un terrain plat
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
			for (int x = 0; x < CHUNK_SIZE_X; ++x)
				for (int z = 0; z < CHUNK_SIZE_Z; ++z)
					for (int y = 0; y < CHUNK_SIZE_Y; ++y)
						if (y < 64 && y >= 63)
							m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_GRASS);
						else if (y < 63 && y >= 57)
							m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_DIRT);
						else if (y < 57 && y > 40)
							m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_STONE);
						else if (y <= 40 && y > 38)
							m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_BED_ROCK);
}

BlockType World::BlockAt(float x, float y, float z)
{
	Vector3<float>chunkPos(floor(x / CHUNK_SIZE_X), 0, floor(z / CHUNK_SIZE_Z));

	if (chunkPos.x >= 0 && chunkPos.z >= 0 && chunkPos.x < WORLD_SIZE && chunkPos.z < WORLD_SIZE)
	{

		Vector3<float>blockPos(x - (chunkPos.x * CHUNK_SIZE_X), y, z - (chunkPos.z * CHUNK_SIZE_X));

		return m_chunks.Get(chunkPos.x, chunkPos.z).GetBlock(blockPos.x, blockPos.y, blockPos.z);
	}
	else
		return BTYPE_AIR;
}

Chunk& World::ChunkAt(float x, float z)
{
	return m_chunks.Get(x, z);

}

