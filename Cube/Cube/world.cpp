#include "world.h"

World::World() : m_chunks(WORLD_SIZE, WORLD_SIZE), m_seed(15)
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

void World::InitMap(int seed)
{
	m_seed = seed;
	std::srand(seed);

	//seed simplex noise
	for (int i = 0; i < 512; i++)
	{
		perm[i] = rand() % 256;
	}

	//Erase map
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
			for (int x = 0; x < CHUNK_SIZE_X; ++x)
				for (int z = 0; z < CHUNK_SIZE_Z; ++z)
					for (int y = 0; y < CHUNK_SIZE_Y; ++y)
						m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_AIR);

	int count = 0;

	//Map height
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
		{
			for (int x = 0; x < CHUNK_SIZE_X; ++x)
				for (int z = 0; z < CHUNK_SIZE_Z; ++z)
				{
					float val = scaled_octave_noise_2d(8, 0.1, (seed) ? 20 : 0, -50, 20, (float)(i * CHUNK_SIZE_X + x) / 2000.f, (float)(j * CHUNK_SIZE_Z + z) / 2000.f);
					//Couche
					for (int y = 0; y <= CHUNK_SIZE_Y; y++)
					{

						if (y == 0)
							m_chunks.Get(i, j).SetBlock(x, val + 80 - y, z, BTYPE_GRASS);
						else if (y >= 1 && y < 4)
							m_chunks.Get(i, j).SetBlock(x, val + 80 - y, z, BTYPE_DIRT);
						else if (y >= 4)
							m_chunks.Get(i, j).SetBlock(x, val + 80 - y, z, BTYPE_STONE);
					}
					//Plancher de bedrock
					m_chunks.Get(i, j).SetBlock(x, 0, z, BTYPE_BED_ROCK);
					m_chunks.Get(i, j).SetBlock(x, 1, z, BTYPE_BED_ROCK);

					m_chunks.Get(i, j).SetBlock(x, 2, z, BTYPE_STONE);
					m_chunks.Get(i, j).SetBlock(x, 2, z, BTYPE_STONE);

				}
			if (count++ % 300 == 0)
				std::cout << "Chunk " << count << " / " << WORLD_SIZE * WORLD_SIZE << " Created" << std::endl;


		}

	if (seed > 0)
	{
		//Minerals
		/*
		Coal:
		layer range: 1 - 128
		Percentage : 1.19%

		Iron:
		layer Range : 1 - 65
		Percentage : 0.68%

		Gold:
		layer range : 1 - 33
		Percentage : 0.13%

		Redstone:
		Layer Range : 1 - 15
		Percentages by layer : 0.98%

		Lapis Lazuli:
		Layer Range : 1 - 33
		Percentage : 0.13%

		Diamond:
		Layer Range : 1 - 15
		Percentage : 0.12%
		*/

		std::cout << "Adding minerals..." << std::endl;

		for (int i = 0; i < WORLD_SIZE; i++)
			for (int j = 0; j < WORLD_SIZE; j++)
				for (int x = 0; x < CHUNK_SIZE_X; ++x)
					for (int z = 0; z < CHUNK_SIZE_Z; ++z)
						for (int y = 3; y <= CHUNK_SIZE_Y; y++)
						{
							if (rand() % 10000 >= 9891 && m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_STONE)
							{
								AddMineral(BTYPE_COAL, i, j, x, y, z);
							}
							else if (rand() % 10000 >= 9932 && m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_STONE && y < 66)
							{
								AddMineral(BTYPE_IRON, i, j, x, y, z);
							}
							else if (rand() % 10000 >= 9987 && m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_STONE && y < 34)
							{
								AddMineral(BTYPE_GOLD, i, j, x, y, z);
							}
							else if (rand() % 10000 >= 9907 && m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_STONE && y < 16)
							{
								AddMineral(BTYPE_REDSTONE, i, j, x, y, z);
							}
							else if (rand() % 10000 >= 9987 && m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_STONE && y < 34)
							{
								AddMineral(BTYPE_LAPIS_LAZULI, i, j, x, y, z);
							}
							else if (rand() % 10000 >= 9988 && m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_STONE && y < 16)
							{
								AddMineral(BTYPE_DIAMOND, i, j, x, y, z);
							}

						}

		//Cave

		std::cout << "Adding caves..." << std::endl;

		//Nombre de caverne
		for (int i = 0; i < rand() % (WORLD_SIZE * 10) + (WORLD_SIZE * 2); i++)
		{
			Vector3<float> head(rand() % (WORLD_SIZE*CHUNK_SIZE_X), rand() % (CHUNK_SIZE_Y - 40), rand() % (WORLD_SIZE*CHUNK_SIZE_X));

			//Longeur d'une tunel
			for (int j = 0; j < rand() % 300 + 200; j++)
			{
				Vector3<float> chunkPos(floor((head.x) / CHUNK_SIZE_X), 0, floor((head.z) / CHUNK_SIZE_Z));

				//Si on est dans la map
				if (chunkPos.x >= 0 && chunkPos.z >= 0 && chunkPos.x < WORLD_SIZE && chunkPos.z < WORLD_SIZE)
				{
					//Largeur du tunel
					for (int q = 0; q < rand() % 2 + 4; q++)
					{
						for (int w = 0; w < rand() % 2 + 4; w++)
						{
							for (int e = 0; e < rand() % 2 + 4; e++)
							{
								Vector3<float> blockPos(head.x - (chunkPos.x * CHUNK_SIZE_X) + q, head.y + w, head.z - (chunkPos.z * CHUNK_SIZE_X) + e);

								//Set le bloc a air
								m_chunks.Get(chunkPos.x, chunkPos.z).SetBlock(blockPos.x, blockPos.y, blockPos.z, BTYPE_AIR);
							}
						}
					}
				}

				//On avance la tete du tunel
				head.x += (rand() % 100 > 50) ? 1 : -1;
				head.y += (rand() % 100 > 50) ? 1 : -1;
				head.z += (rand() % 100 > 50) ? 1 : -1;
			}
		}
		//Water
		std::cout << "Adding ocean..." << std::endl;

		for (int i = 0; i < WORLD_SIZE; i++)
			for (int j = 0; j < WORLD_SIZE; j++)
				for (int x = 0; x < CHUNK_SIZE_X; x++)
					for (int z = 0; z < CHUNK_SIZE_Z; z++)
						for (int y = 0; y < 60; y++)
						{

							if (m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_AIR &&
								(m_chunks.Get(i, j).GetBlock(x, y - 1, z) == BTYPE_GRASS ||
								m_chunks.Get(i, j).GetBlock(x, y - 1, z) == BTYPE_DIRT ||
								m_chunks.Get(i, j).GetBlock(x, y - 1, z) == BTYPE_WATER))
							{
								m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_WATER);
							}

						}

		//Tree
		std::cout << "Adding trees..." << std::endl;

		for (int i = 0; i < WORLD_SIZE; i++)
			for (int j = 0; j < WORLD_SIZE; j++)
				for (int x = 0; x < CHUNK_SIZE_X; x += 2)
					for (int z = 0; z < CHUNK_SIZE_Z; z += 2)
					{

						int y = 128;
						if (rand() % 100 >= 90)
						{
							//Trouve le grass le plus haut et ajoute l'arbre acette position
							while (m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_AIR)
							{
								y--;

							}
							if (m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_GRASS &&
								m_chunks.Get(i, j).GetBlock(x + 1, y, z) == BTYPE_GRASS &&
								m_chunks.Get(i, j).GetBlock(x - 1, y, z) == BTYPE_GRASS &&
								m_chunks.Get(i, j).GetBlock(x, y, z + 1) == BTYPE_GRASS &&
								m_chunks.Get(i, j).GetBlock(x, y, z - 1) == BTYPE_GRASS)
							{
								y++;
								AddTree(i, j, x, y, z);
							}
						}
					}
		


		//Little fix
		std::cout << "Fixing..." << std::endl;
		for (int i = 0; i < WORLD_SIZE; i++)
			for (int j = 0; j < WORLD_SIZE; j++)
				for (int x = 0; x < CHUNK_SIZE_X; ++x)
					for (int z = 0; z < CHUNK_SIZE_Z; ++z)
						for (int y = 3; y <= CHUNK_SIZE_Y; y++)
						{
							if (m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_DIRT && m_chunks.Get(i, j).GetBlock(x, y + 1, z) == BTYPE_AIR)
							{
								m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_GRASS);
							}
							if (m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_GRASS && y < 65)
							{
								m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_SAND);
								m_chunks.Get(i, j).SetBlock(x, y - 1, z, BTYPE_SAND);
								m_chunks.Get(i, j).SetBlock(x, y - 2, z, BTYPE_SAND);
								m_chunks.Get(i, j).SetBlock(x, y - 3, z, BTYPE_SAND);
							}
							else if (m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_GRASS && m_chunks.Get(i, j).GetBlock(x, y +1, z) == BTYPE_WATER)
							{
								m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_SAND);
								m_chunks.Get(i, j).SetBlock(x, y - 1, z, BTYPE_SAND);
								m_chunks.Get(i, j).SetBlock(x, y - 2, z, BTYPE_SAND);
								m_chunks.Get(i, j).SetBlock(x, y - 3, z, BTYPE_SAND);
							}
						}


	}

	if (seed != 0)
		std::cout << "Map created with this seed: " << seed << std::endl << std::endl;
	else
		std::cout << "Flat map created" << std::endl << std::endl;
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
	std::cout << "Loading " << filename << "..." << std::endl;

	//Erase map
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
			for (int x = 0; x < CHUNK_SIZE_X; ++x)
				for (int z = 0; z < CHUNK_SIZE_Z; ++z)
					for (int y = 0; y < CHUNK_SIZE_Y; ++y)
					{
						m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_AIR);
						m_chunks.Get(i, j).SetBlock(0, 0, 0, BTYPE_TEST);
						m_chunks.Get(i, j).GetSave() = false;
					}

	//Open file
	std::ifstream file;
	file.open(filename.c_str());

	//Chunk pos, block pos, blocktype
	int i, j, x, y, z, b;

	//Read All file
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::stringstream ss(str);

	//Close it
	file.close();

	//Get Size
	ss.seekg(0, file.end);
	float length = ss.tellg() / 1024;
	ss.seekg(0, file.beg);

	ss >> m_seed;

	InitMap(m_seed);

	//Read
	while (ss)
	{
		//Get the value for a block
		ss >> i >> j >> x >> y >> z >> b;

		if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z
			&& j >= 0 && i >= 0 && j < WORLD_SIZE  && i < WORLD_SIZE
			&& b >= 0 && b < NUMBER_OF_BLOCK)
		{
			//Set block

			if (b == 0)
				m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_AIR);

			else
				m_chunks.Get(i, j).SetBlock(x, y, z, binfo[b].GetType());

			m_chunks.Get(i, j).GetSave() = true;

		}
		//Tell Where we are
		if (ss.tellg() % 1024 == 0)
			std::cout << float(ss.tellg() / 1024) << " / " << length << " KB loaded" << std::endl;

	}

	std::cout << filename << " Loaded" << std::endl << std::endl;
}

void World::SaveMap(std::string filename)
{
	std::ofstream file;
	file.open(filename.c_str());
	int count = 1;
	int total = 0;

	std::cout << "Saving " << filename << "..." << std::endl;

	//Compte combien de chunk a besoin d'etre sauvegarder
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
			if (ChunkAt(i, j).GetSave())
				total++;

	file << m_seed << std::endl;

	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
		{
			//Si le chunk a besoin d'etre sauvegarder
			if (ChunkAt(i, j).GetSave())
			{
				for (int x = 0; x < CHUNK_SIZE_X; ++x)

					for (int z = 0; z < CHUNK_SIZE_Z; ++z)
						for (int y = 0; y <= CHUNK_SIZE_Y; y++)
						{

							file << i << " " << j << " " << x << " " << y << " " << z << " " << (int)m_chunks.Get(i, j).GetBlock(x, y, z) << " ";
						}

				std::cout << "Chunk " << count++ << " / " << total << " saved" << std::endl;
				file << std::endl;
			}
		}
	file << "END " << std::endl;
	file.close();

	std::cout << "Map saved as " << filename << std::endl << std::endl;
}

void World::AddMineral(BlockType mineral, int i, int j, int x, int y, int z)
{

	m_chunks.Get(i, j).SetBlock(x, y, z, mineral);
	if (rand() % 100 >= 60 && m_chunks.Get(i, j).GetBlock(x + 1, y, z) == BTYPE_STONE)
	{
		m_chunks.Get(i, j).SetBlock(x + 1, y, z, mineral);
	}
	if (rand() % 100 >= 60 && m_chunks.Get(i, j).GetBlock(x - 1, y, z) == BTYPE_STONE)
	{
		m_chunks.Get(i, j).SetBlock(x - 1, y, z, mineral);
	}
	if (rand() % 100 >= 60 && m_chunks.Get(i, j).GetBlock(x, y + 1, z) == BTYPE_STONE)
	{
		m_chunks.Get(i, j).SetBlock(x, y + 1, z, mineral);
	}
	if (rand() % 100 >= 60 && m_chunks.Get(i, j).GetBlock(x, y - 1, z) == BTYPE_STONE)
	{
		m_chunks.Get(i, j).SetBlock(x, y - 1, z, mineral);
	}
	if (rand() % 100 >= 60 && m_chunks.Get(i, j).GetBlock(x, y, z + 1) == BTYPE_STONE)
	{
		m_chunks.Get(i, j).SetBlock(x, y, z + 1, mineral);
	}
	if (rand() % 100 >= 60 && m_chunks.Get(i, j).GetBlock(x, y, z - 1) == BTYPE_STONE)
	{
		m_chunks.Get(i, j).SetBlock(x, y, z - 1, mineral);
	}
}

void World::AddTree(int i, int j, int x, int y, int z)
{
	int hauteur = rand() % 7 + 3;

	for (int k = 0; k < hauteur; k++)
	{
		m_chunks.Get(i, j).SetBlock(x, y + k, z, BTYPE_WOOD);
	}


	m_chunks.Get(i, j).SetBlock(x + 1, y + hauteur - 1, z, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x + 1, y + hauteur - 1, z + 1, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x, y + hauteur - 1, z + 1, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x, y + hauteur - 1, z - 1, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x - 1, y + hauteur - 1, z - 1, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x - 1, y + hauteur - 1, z, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x - 1, y + hauteur - 1, z + 1, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x + 1, y + hauteur - 1, z - 1, BTYPE_LEAVE);

	m_chunks.Get(i, j).SetBlock(x + 1, y + hauteur, z, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x, y + hauteur, z + 1, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x, y + hauteur, z - 1, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x - 1, y + hauteur, z, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x, y + hauteur, z, BTYPE_LEAVE);
	m_chunks.Get(i, j).SetBlock(x, y + hauteur + 1, z, BTYPE_LEAVE);
}

void World::Update(int CenterX, int CenterZ, BlockInfo* info)
{
	if (CenterX >= 0 && CenterZ >= 0 && CenterX < WORLD_SIZE && CenterZ < WORLD_SIZE)
		if (ChunkAt(CenterX, CenterZ).IsDirty())
			ChunkAt(CenterX, CenterZ).Update(info);

	//Parcours les chunk en cercle
	for (int x = 1; x < RENDER_DISTANCE + 1; ++x)
	{
		for (int a = -x; a <= x; ++a)
		{
			Vector3<float> chunkPos(CenterX + a, 0, CenterZ - x);
			//Si le chunk existe
			if (chunkPos.x >= 0 && chunkPos.z >= 0 && chunkPos.x < WORLD_SIZE && chunkPos.z < WORLD_SIZE)
				//Si dirty
				if (ChunkAt(chunkPos.x, chunkPos.z).IsDirty())
				{
					ChunkAt(chunkPos.x, chunkPos.z).Update(info);
					return;
				}
		}
		for (int a = -x; a <= x; ++a)
		{
			Vector3<float> chunkPos(CenterX + a, 0, CenterZ + x);
			//Si le chunk existe
			if (chunkPos.x >= 0 && chunkPos.z >= 0 && chunkPos.x < WORLD_SIZE && chunkPos.z < WORLD_SIZE)
				//Si dirty
				if (ChunkAt(chunkPos.x, chunkPos.z).IsDirty())
				{
					ChunkAt(chunkPos.x, chunkPos.z).Update(info);
					return;
				}
		}
		for (int a = -x; a <= x; ++a)
		{
			Vector3<float> chunkPos(CenterX - x, 0, CenterZ + a);
			//Si le chunk existe
			if (chunkPos.x >= 0 && chunkPos.z >= 0 && chunkPos.x < WORLD_SIZE && chunkPos.z < WORLD_SIZE)
				//Si dirty
				if (ChunkAt(chunkPos.x, chunkPos.z).IsDirty())
				{
					ChunkAt(chunkPos.x, chunkPos.z).Update(info);
					return;
				}
		}
		for (int a = -x; a <= x; ++a)
		{
			Vector3<float> chunkPos(CenterX + x, 0, CenterZ + a);
			//Si le chunk existe
			if (chunkPos.x >= 0 && chunkPos.z >= 0 && chunkPos.x < WORLD_SIZE && chunkPos.z < WORLD_SIZE)
				//Si dirty
				if (ChunkAt(chunkPos.x, chunkPos.z).IsDirty())
				{
					ChunkAt(chunkPos.x, chunkPos.z).Update(info);
					return;
				}
		}
	}
}
