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

void World::InitFlatMap()
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

// - octaves: smoothness
// - freq: Agressivité des montagnes
// - Amp: Hauteur des montagnes
// - Seed: seed pour le random
void World::InitMap(int octaves, float freq, float amp, int seed)
{
	Perlin perlin(octaves, freq, amp, seed);


	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
			for (int x = 0; x < CHUNK_SIZE_X; ++x)
				for (int z = 0; z < CHUNK_SIZE_Z; ++z)
				{
					float val = perlin.Get((float)(i * CHUNK_SIZE_X + x) / 2000.f, (float)(j * CHUNK_SIZE_Z + z) / 2000.f);
					
					for (int y = 0; y <= 100; y++)
					{
						if (y == 0)
							m_chunks.Get(i, j).SetBlock(x, val + 64 - y, z, BTYPE_GRASS);
						else if (y >= 1 && y < 4)
							m_chunks.Get(i, j).SetBlock(x, val + 64 - y, z, BTYPE_DIRT);
						else if (y >= 4)
							m_chunks.Get(i, j).SetBlock(x, val + 64 - y, z, BTYPE_STONE);


					}
					//Plancher de bedrock
					m_chunks.Get(i, j).SetBlock(x, 0, z, BTYPE_BED_ROCK);
					m_chunks.Get(i, j).SetBlock(x, 1, z, BTYPE_BED_ROCK);

					m_chunks.Get(i, j).SetBlock(x, 0, z, BTYPE_STONE);
					m_chunks.Get(i, j).SetBlock(x, 1, z, BTYPE_STONE);

				}
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

