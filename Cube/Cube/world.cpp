#include "world.h"

World::World() : m_chunks(WORLD_SIZE, WORLD_SIZE), m_seed(6)
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
	randomize();

	
	//Erase map
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
		{

			m_chunks.Get(i, j).GetSave() = false;
			m_chunks.Get(i, j).DeleteCache();
			m_chunks.Get(i, j).m_iscreated = false;

		}
	

	if (seed != 0)
		std::cout << "Map created with this seed: " << seed << std::endl << std::endl;
	else
		std::cout << "Flat map created" << std::endl << std::endl;
}

void World::InitChunk(float i, float j)
{
	m_chunks.Get(i, j).m_iscreated = true;
	std::srand(m_seed * i * j);

	for (int x = 0; x < CHUNK_SIZE_X; ++x)
		for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			for (int y = 0; y < CHUNK_SIZE_Y; ++y)
			{
				if (m_chunks.Get(i, j).GetBlock(x, y, z) != BTYPE_AIR)
					m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_AIR);

			}

	//Height
	for (int x = 0; x < CHUNK_SIZE_X; ++x)
		for (int z = 0; z < CHUNK_SIZE_Z; ++z)
		{
			float scale = scaled_octave_noise_2d(8, 0.03f, 20, 10, 10, (float)(i * CHUNK_SIZE_X + x) / 5000.f, (float)(j * CHUNK_SIZE_Z + z) / 5000.f);
			float val = scaled_octave_noise_2d(8, 0.3f, (m_seed) ? scale : 0, -50, 20, (float)(i * CHUNK_SIZE_X + x) / 1000.f, (float)(j * CHUNK_SIZE_Z + z) / 1000.f);
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
	if (m_seed > 0)
	{
		//Minerals
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


		//Water
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

		//Sand
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
				for (int y = 3; y <= CHUNK_SIZE_Y; y++)
				{
					if (m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_GRASS && y < 65)
					{
						m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_SAND);
						m_chunks.Get(i, j).SetBlock(x, y - 1, z, BTYPE_SAND);
						m_chunks.Get(i, j).SetBlock(x, y - 2, z, BTYPE_SAND);
						m_chunks.Get(i, j).SetBlock(x, y - 3, z, BTYPE_SAND);
					}
					else if (m_chunks.Get(i, j).GetBlock(x, y, z) == BTYPE_GRASS && m_chunks.Get(i, j).GetBlock(x, y + 1, z) == BTYPE_WATER)
					{
						m_chunks.Get(i, j).SetBlock(x, y, z, BTYPE_SAND);
						m_chunks.Get(i, j).SetBlock(x, y - 1, z, BTYPE_SAND);
						m_chunks.Get(i, j).SetBlock(x, y - 2, z, BTYPE_SAND);
						m_chunks.Get(i, j).SetBlock(x, y - 3, z, BTYPE_SAND);
					}
				}
		
		//Cave
		if (rand() % 100 > 90)
		{
			Vector3<float> head(rand() % (CHUNK_SIZE_X)+(i * CHUNK_SIZE_X), rand() % (CHUNK_SIZE_Y - 40), rand() % (CHUNK_SIZE_Z)+(j * CHUNK_SIZE_Z));

			//Longeur d'une tunel
			for (int g = 0; g < rand() % 300 + 200; g++)
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
								if (m_chunks.Get(chunkPos.x, chunkPos.z).GetBlock(blockPos.x, blockPos.y, blockPos.z) != BTYPE_WATER &&
									m_chunks.Get(chunkPos.x, chunkPos.z).GetBlock(blockPos.x, blockPos.y, blockPos.z) != BTYPE_SAND && 
									!m_chunks.Get(chunkPos.x, chunkPos.z).GetSave()
									)
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
		
		//Tree
		for (int x = 0; x < CHUNK_SIZE_X; x += 2)
			for (int z = 0; z < CHUNK_SIZE_Z; z += 2)
			{

				int y = 128;
				if (rand() % 100 >= 75)
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


	}
}

BlockType World::BlockAt(float x, float y, float z) const
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

Chunk* World::ChunkAt(float x, float z)
{
	if (x >= 0 && z >= 0 && x < WORLD_SIZE && z < WORLD_SIZE)
		return &m_chunks.Get(x, z);
	else
		return NULL;
}

void World::LoadMap(std::string filename, BlockInfo* &binfo)
{
	std::cout << "Loading " << filename << "..." << std::endl;

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

	ss >> m_seed;

	InitMap(m_seed);

	//Read
	while (ss)
	{	
		i = -1;
		j = -1;
		std::string line;
		std::getline(ss, line, '\n');
		std::stringstream ssline(line);


		//Get the value for a block
		ssline >> i >> j;
		if (j >= 0 && i >= 0 && j < WORLD_SIZE  && i < WORLD_SIZE)
		{
			Chunk* chunk = ChunkAt(i, j);
			chunk->GetSave() = true;
			chunk->m_iscreated = true;
			for (int x = 0; x < CHUNK_SIZE_X; ++x)
				for (int z = 0; z < CHUNK_SIZE_Z; ++z)
					for (int y = 0; y <= CHUNK_SIZE_Y; y++)
					{
						ssline >> b;
						if (b >= 0 && b < NUMBER_OF_BLOCK)				
							chunk->SetBlock(x, y, z, (b == 0) ? BTYPE_AIR : binfo[b].GetType());

					}
		}	
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
			if (ChunkAt(i, j)->GetSave())
				total++;

	file << m_seed << std::endl;

	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
		{

			//Si le chunk a besoin d'etre sauvegarder
			if (ChunkAt(i, j)->GetSave())
			{
				file << i << " " << j << " ";
				for (int x = 0; x < CHUNK_SIZE_X; ++x)
					for (int z = 0; z < CHUNK_SIZE_Z; ++z)
						for (int y = 0; y <= CHUNK_SIZE_Y; y++)
							file << (int)m_chunks.Get(i, j).GetBlock(x, y, z) << " ";


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

void World::Update(int CenterX, int CenterZ, BlockInfo* &info)
{
	//Si n'est pas creer
	if (ChunkAt(CenterX, CenterZ) && !ChunkAt(CenterX, CenterZ)->m_iscreated)
		InitChunk(CenterX, CenterZ);
	//Si dirty
	if (ChunkAt(CenterX, CenterZ) && ChunkAt(CenterX, CenterZ)->NeedUpdate())
		ChunkAt(CenterX, CenterZ)->Update(info);

	//Parcours les chunk en cercle
	for (int x = 1; x < RENDER_DISTANCE + 1; ++x)
	{
		for (int a = -x; a <= x; ++a)
		{
			Vector3<float> chunkPos(CenterX + a, 0, CenterZ - x);
			//Si n'est pas creer
			if (ChunkAt(chunkPos.x, chunkPos.z) && !ChunkAt(chunkPos.x, chunkPos.z)->m_iscreated)
				InitChunk(chunkPos.x, chunkPos.z);
			//Si dirty
			if (ChunkAt(chunkPos.x, chunkPos.z) && ChunkAt(chunkPos.x, chunkPos.z)->NeedUpdate())
			{
				ChunkAt(chunkPos.x, chunkPos.z)->Update(info);
				return;
			}
		}
		for (int a = -x; a <= x; ++a)
		{
			Vector3<float> chunkPos(CenterX + a, 0, CenterZ + x);
			//Si n'est pas creer
			if (ChunkAt(chunkPos.x, chunkPos.z) && !ChunkAt(chunkPos.x, chunkPos.z)->m_iscreated)
				InitChunk(chunkPos.x, chunkPos.z);
			//Si dirty
			if (ChunkAt(chunkPos.x, chunkPos.z) && ChunkAt(chunkPos.x, chunkPos.z)->NeedUpdate())
			{
				ChunkAt(chunkPos.x, chunkPos.z)->Update(info);
				return;
			}
		}
		for (int a = -x; a <= x; ++a)
		{
			Vector3<float> chunkPos(CenterX - x, 0, CenterZ + a);
			//Si n'est pas creer
			if (ChunkAt(chunkPos.x, chunkPos.z) && !ChunkAt(chunkPos.x, chunkPos.z)->m_iscreated)
				InitChunk(chunkPos.x, chunkPos.z);
			//Si dirty
			if (ChunkAt(chunkPos.x, chunkPos.z) && ChunkAt(chunkPos.x, chunkPos.z)->NeedUpdate())
			{
				ChunkAt(chunkPos.x, chunkPos.z)->Update(info);
				return;
			}
		}
		for (int a = -x; a <= x; ++a)
		{
			Vector3<float> chunkPos(CenterX + x, 0, CenterZ + a);
			//Si n'est pas creer
			if (ChunkAt(chunkPos.x, chunkPos.z) && !ChunkAt(chunkPos.x, chunkPos.z)->m_iscreated)
				InitChunk(chunkPos.x, chunkPos.z);
			//Si dirty
			if (ChunkAt(chunkPos.x, chunkPos.z) && ChunkAt(chunkPos.x, chunkPos.z)->NeedUpdate())
			{
				ChunkAt(chunkPos.x, chunkPos.z)->Update(info);
				return;
			}
		}
	}


}

int World::ChunkNotUpdated(int CenterX, int CenterZ)
{
	int chunkNotUpdated = 0;
	for (int i = 0; i < RENDER_DISTANCE * 2; i++)
		for (int j = 0; j < RENDER_DISTANCE * 2; j++)
		{
			Vector3<int> chunkPos(CenterX + i - RENDER_DISTANCE, 0, CenterZ + j - RENDER_DISTANCE);

			if (ChunkAt(chunkPos.x, chunkPos.z) && ChunkAt(chunkPos.x, chunkPos.z)->NeedUpdate())
				chunkNotUpdated++;

		}
	return chunkNotUpdated;
}

void World::Render(int CenterX, int CenterZ, GLenum &m_program)
{

	//Render les blocks
	for (int i = 0; i < RENDER_DISTANCE * 2; i++)
		for (int j = 0; j < RENDER_DISTANCE * 2; j++)
		{
			Vector3<int> chunkPos(CenterX + i - RENDER_DISTANCE, 0, CenterZ + j - RENDER_DISTANCE);

			//Si le chunk existe on le render
			if (ChunkAt(chunkPos.x, chunkPos.z))
			{
				ChunkAt(chunkPos.x, chunkPos.z)->RenderSolidBuffer(m_program);

			}

		}

	//Render le transparent (ex: BTYPE_WATER)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < RENDER_DISTANCE * 2; i++)
		for (int j = 0; j < RENDER_DISTANCE * 2; j++)
		{
			Vector3<int> chunkPos(CenterX + i - RENDER_DISTANCE, 0, CenterZ + j - RENDER_DISTANCE);

			//Si le chunk existe on le render
			if (ChunkAt(chunkPos.x, chunkPos.z))
				ChunkAt(chunkPos.x, chunkPos.z)->RenderTransparentBuffer(m_program);
		}
	glDisable(GL_BLEND);
}
