#include "world.h"


World::World() : m_chunks(WORLD_SIZE, WORLD_SIZE)
{
	//Parcours les chunks et les positionne dans la map
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
		{
			m_chunks.Get(i, j).SetPosition(CHUNK_SIZE_X * i, 0, CHUNK_SIZE_Z * j);

			//Set les pointeur sur les chunk qui est collé
			m_chunks.Get(i, j).m_positiveX = NULL;
			m_chunks.Get(i, j).m_negativeX = NULL;
			m_chunks.Get(i, j).m_positiveZ = NULL;
			m_chunks.Get(i, j).m_negativeZ = NULL;

			if (i < WORLD_SIZE - 1)
				m_chunks.Get(i, j).m_positiveX = &m_chunks.Get(i + 1, j);

			if (i > 0)
				m_chunks.Get(i, j).m_negativeX = &m_chunks.Get(i - 1, j);

			if (j < WORLD_SIZE - 1)
				m_chunks.Get(i, j).m_positiveZ = &m_chunks.Get(i, j + 1);

			if (j > 0)
				m_chunks.Get(i, j).m_negativeZ = &m_chunks.Get(i, j - 1);
		}


}


World::~World()
{
}


void World::InitMap(int octaves, float freq, float amp, int seed)
{
	//Erase map
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
			for (int x = 0; x < CHUNK_SIZE_X; ++x)
				for (int z = 0; z < CHUNK_SIZE_Z; ++z)
					for (int y = 0; y < CHUNK_SIZE_Y; ++y)
						m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_AIR);

	Perlin perlin(octaves, freq, amp, seed);

	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
			for (int x = 0; x < CHUNK_SIZE_X; ++x)
				for (int z = 0; z < CHUNK_SIZE_Z; ++z)
				{
					float val = perlin.Get((float)(i * CHUNK_SIZE_X + x) / 2000.f, (float)(j * CHUNK_SIZE_Z + z) / 2000.f);

					//Couche
					for (int y = 0; y <= CHUNK_SIZE_Y; y++)
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

					m_chunks.Get(i, j).SetBlock(x, 2, z, BTYPE_STONE);
					m_chunks.Get(i, j).SetBlock(x, 2, z, BTYPE_STONE);

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
	if (x >= 0 && z >= 0 && x < WORLD_SIZE  && z < WORLD_SIZE)
		return m_chunks.Get(x, z);



}

void World::LoadMap(std::string filename, BlockInfo *binfo)
{
	//Erase map
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
			for (int x = 0; x < CHUNK_SIZE_X; ++x)
				for (int z = 0; z < CHUNK_SIZE_Z; ++z)
					for (int y = 0; y < CHUNK_SIZE_Y; ++y)
					{
						m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_AIR);
						m_chunks.Get(i, j).SetBlock(0, 0, 0, BTYPE_TEST);
					}



	//Open file
	std::ifstream file;
	file.open(filename, std::ios::in);

	//Chunk pos, block pos, blocktype
	int i, j, x, y, z, b;

	//Number of block loaded
	int count = 1;
	int total = 0;

	file.seekg(0, file.end);
	float length = file.tellg() / 1024;
	file.seekg(0, file.beg);

	std::string word;

	//Read
	while (file)
	{
		count++;

		//Read word and convert in int
		file >> word;
		std::istringstream(word) >> i;

		file >> word;
		std::istringstream(word) >> j;

		file >> word;
		std::istringstream(word) >> x;

		file >> word;
		std::istringstream(word) >> y;

		file >> word;
		std::istringstream(word) >> z;

		file >> word;
		std::istringstream(word) >> b;

		if (b >= 0 && b << NUMBER_OF_BLOCK)
			m_chunks.Get(i, j).SetBlock(x, y, z, binfo[b].GetType());

		if (count % 10000 == 0)
			std::cout << float(file.tellg() / 1024) << " / " << length << " KB loaded" << std::endl;

	}
	
	file.close();
	std::cout << "Map Loaded" << std::endl;
}

void World::SaveMap(std::string filename)
{
	std::ofstream file;
	file.open(filename);
	int count = 1;

	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
		{
			for (int x = 0; x < CHUNK_SIZE_X; ++x)

				for (int z = 0; z < CHUNK_SIZE_Z; ++z)
					for (int y = 0; y <= CHUNK_SIZE_Y; y++)
					{
						if (m_chunks.Get(i, j).GetBlock(x, y, z) != BTYPE_AIR)
							file << i << " " << j << " " << x << " " << y << " " << z << " " << (int)m_chunks.Get(i, j).GetBlock(x, y, z) << " ";
					}

			std::cout << "Chunk " << count++ << " / " << WORLD_SIZE * WORLD_SIZE << " saved" << std::endl;
			file << std::endl;
		}
	file << "END " << std::endl;
	file.close();

	std::cout << "Map saved" << std::endl;
}

