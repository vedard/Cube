#include "world.h"

World::World() : m_chunks(WORLD_SIZE, WORLD_SIZE), m_seed(6)
{
	//Parcours les chunks et les positionne dans la map
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
		{
			Chunk * chunk = ChunkAt(i, j);

			chunk->SetPosition(CHUNK_SIZE_X * i, 0, CHUNK_SIZE_Z * j);

			//Set les pointeur sur les chunk qui est collé
			chunk->m_positiveX = NULL;
			chunk->m_negativeX = NULL;
			chunk->m_positiveZ = NULL;
			chunk->m_negativeZ = NULL;

			if (i < WORLD_SIZE - 1)
				chunk->m_positiveX = ChunkAt(i + 1, j);

			if (i > 0)
				chunk->m_negativeX = ChunkAt(i - 1, j);

			if (j < WORLD_SIZE - 1)
				chunk->m_positiveZ = ChunkAt(i, j + 1);

			if (j > 0)
				chunk->m_negativeZ = ChunkAt(i, j - 1);
		}


}

World::~World()
{
}

void World::InitMap(int seed)
{

	m_seed = seed;
	std::srand(seed);
	randomize(seed);


	//Erase map
	for (int i = 0; i < WORLD_SIZE; i++)
		for (int j = 0; j < WORLD_SIZE; j++)
		{
			Chunk * chunk = ChunkAt(i, j);
			chunk->GetSave() = false;
			chunk->DeleteCache();
			chunk->m_iscreated = false;

		}


	if (seed != 0)
		std::cout << "Map created with this seed: " << seed << std::endl << std::endl;
	else
		std::cout << "Flat map created" << std::endl << std::endl;
}

void World::InitChunk(float i, float j)
{
	Chunk* chunk = ChunkAt(i, j);
	chunk->m_iscreated = true;
	std::srand(m_seed * (i + 1) * (j + 1));

	for (int x = 0; x < CHUNK_SIZE_X; ++x)
		for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			for (int y = 0; y < CHUNK_SIZE_Y; ++y)
			{
				if (chunk->GetBlock(x, y, z) != BTYPE_AIR)
					chunk->SetBlock(x, y, z, BTYPE_AIR);

			}
	//Height
	for (int x = 0; x < CHUNK_SIZE_X; x++)
		for (int z = 0; z < CHUNK_SIZE_Z; z++)
		{

			float biome = scaled_octave_noise_2d(14, 0.3f, 5, 0.001, 1, (float)(i * CHUNK_SIZE_X + x) / 40000, (float)(j * CHUNK_SIZE_Z + z) / 40000);
			float val;

			val = scaled_octave_noise_2d(16, 0.3f, (m_seed) ? 7 : 0, -50, 40, (float)(i * CHUNK_SIZE_X + x) / (5000 * biome), (float)(j * CHUNK_SIZE_Z + z) / (5000 * biome));

			//Couche
			for (int y = 0; y <= 200; y++)
			{
				int height = val + 160 - y;

				if (height > 64)
				{

					if (y == 0)
						chunk->SetBlock(x, height, z, BTYPE_GRASS);
					else if (y >= 1 && y < 4)
						chunk->SetBlock(x, height, z, BTYPE_DIRT);
					else if (y >= 4)
						chunk->SetBlock(x, height, z, BTYPE_STONE);
				}
			}
			val = scaled_octave_noise_2d(16, 0.3f, (m_seed) ? 7 : 0, -1, 1, (float)(i * CHUNK_SIZE_X + x) / (5000 * biome), (float)(j * CHUNK_SIZE_Z + z) / (5000 * biome));

			chunk->SetBlock(x, val + 65, z, BTYPE_NETHEREACK);

			val = scaled_octave_noise_2d(15, 0.4f, (m_seed) ? 8 : 0, -40, 35, (float)(i * CHUNK_SIZE_X + x) / (4500 * biome), (float)(j * CHUNK_SIZE_Z + z) / (4500 * biome));


			for (int y = 0; y <= 55; y++)
			{
				chunk->SetBlock(x, val + 20 - y, z, BTYPE_NETHEREACK);
			}
			//Plancher de bedrock
			chunk->SetBlock(x, 0, z, BTYPE_BED_ROCK);
			chunk->SetBlock(x, 1, z, BTYPE_BED_ROCK);

		}

	if (m_seed > 0)
	{
		//Minerals
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
				for (int y = 3; y <= CHUNK_SIZE_Y; y++)
				{
					if (rand() % 10000 >= 9891 && chunk->GetBlock(x, y, z) == BTYPE_STONE)
					{
						AddMineral(BTYPE_COAL, chunk, x, y, z);
					}
					else if (rand() % 10000 >= 9932 && chunk->GetBlock(x, y, z) == BTYPE_STONE && y < 66 + 64)
					{
						AddMineral(BTYPE_IRON, chunk, x, y, z);
					}
					else if (rand() % 10000 >= 9987 && chunk->GetBlock(x, y, z) == BTYPE_STONE && y < 34 + 64)
					{
						AddMineral(BTYPE_GOLD, chunk, x, y, z);
					}
					else if (rand() % 10000 >= 9907 && chunk->GetBlock(x, y, z) == BTYPE_STONE && y < 16 + 64)
					{
						AddMineral(BTYPE_REDSTONE, chunk, x, y, z);
					}
					else if (rand() % 10000 >= 9987 && chunk->GetBlock(x, y, z) == BTYPE_STONE && y < 34 + 64)
					{
						AddMineral(BTYPE_LAPIS_LAZULI, chunk, x, y, z);
					}
					else if (rand() % 10000 >= 9988 && chunk->GetBlock(x, y, z) == BTYPE_STONE && y < 16 + 64)
					{
						AddMineral(BTYPE_DIAMOND, chunk, x, y, z);
					}

				}


		//Water
		for (int x = 0; x < CHUNK_SIZE_X; x++)
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
				for (int y = 0; y < 140; y++)
				{
					if (chunk->GetBlock(x, y, z) == BTYPE_AIR &&
						(chunk->GetBlock(x, y - 1, z) == BTYPE_GRASS ||
						chunk->GetBlock(x, y - 1, z) == BTYPE_DIRT ||
						chunk->GetBlock(x, y - 1, z) == BTYPE_WATER))
					{
						chunk->SetBlock(x, y, z, BTYPE_WATER);
					}

				}

		//Lava
		for (int x = 0; x < CHUNK_SIZE_X; x++)
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
				for (int y = 0; y < 7; y++)
				{
					if (chunk->GetBlock(x, y, z) == BTYPE_AIR &&
						(chunk->GetBlock(x, y - 1, z) == BTYPE_NETHEREACK ||
						chunk->GetBlock(x, y - 1, z) == BTYPE_BED_ROCK ||
						chunk->GetBlock(x, y - 1, z) == BTYPE_LAVA))
					{
						chunk->SetBlock(x, y, z, BTYPE_LAVA);
					}

				}

		//Sand
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
				for (int y = 64; y <= CHUNK_SIZE_Y; y++)
				{

					if (chunk->GetBlock(x, y, z) == BTYPE_GRASS && chunk->GetBlock(x, y + 1, z) == BTYPE_WATER)
					{
						chunk->SetBlock(x, y, z, BTYPE_SAND);
						chunk->SetBlock(x, y - 1, z, BTYPE_SAND);
						chunk->SetBlock(x, y - 2, z, BTYPE_SAND);
						chunk->SetBlock(x, y - 3, z, BTYPE_SAND);
						break;
					}
				}

		//Cave
		if (rand() % 100 > 90)
		{
			Vector3<float> head(rand() % (CHUNK_SIZE_X)+(i * CHUNK_SIZE_X), rand() % (CHUNK_SIZE_Y - 100) + 50, rand() % (CHUNK_SIZE_Z)+(j * CHUNK_SIZE_Z));

			//Longeur d'une tunel
			for (int g = 0; g < rand() % 300 + 200; g++)
			{
				Chunk * chunkTemp = ChunkAt(floor((head.x) / CHUNK_SIZE_X), floor((head.z) / CHUNK_SIZE_Z));
				Vector3<float> chunkPos(floor((head.x) / CHUNK_SIZE_X), 0, floor((head.z) / CHUNK_SIZE_Z));

				//Si on est dans la map
				if (chunkTemp)
				{
					//Largeur du tunel
					for (int q = 0; q < rand() % 2 + 4; q++)
					{
						for (int w = 0; w < rand() % 2 + 4; w++)
						{
							for (int e = 0; e < rand() % 2 + 4; e++)
							{
								Vector3<float> blockPos(head.x - (chunkPos.x * CHUNK_SIZE_X) + q, head.y + w, head.z - (chunkPos.z * CHUNK_SIZE_X) + e);
								if (chunkTemp->GetBlock(blockPos.x, blockPos.y, blockPos.z) != BTYPE_WATER &&
									chunkTemp->GetBlock(blockPos.x, blockPos.y, blockPos.z) != BTYPE_SAND &&
									chunkTemp->GetBlock(blockPos.x, blockPos.y, blockPos.z) != BTYPE_BED_ROCK &&
									!chunkTemp->GetSave()
									)
									//Set le bloc a air
									chunkTemp->SetBlock(blockPos.x, blockPos.y, blockPos.z, BTYPE_AIR);
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
				int y = CHUNK_SIZE_Y;

				if (scaled_octave_noise_2d(1, 0.4f, 6, 0, 100, (float)(i * CHUNK_SIZE_X + x) / (5000), (float)(j * CHUNK_SIZE_Z + z) / (5000)) >= 30 && std::rand() % 100 > 90)
				{
					//Trouve le grass le plus haut et ajoute l'arbre a cette position
					while (chunk->GetBlock(x, y, z) == BTYPE_AIR)
						y--;

					if (chunk->GetBlock(x, y, z) == BTYPE_GRASS &&
						chunk->GetBlock(x + 1, y, z) == BTYPE_GRASS &&
						chunk->GetBlock(x - 1, y, z) == BTYPE_GRASS &&
						chunk->GetBlock(x, y, z + 1) == BTYPE_GRASS &&
						chunk->GetBlock(x, y, z - 1) == BTYPE_GRASS)
					{
						y++;
						AddTree(chunk, x, y, z);
					}
				}
			}
	}
}

BlockType World::BlockAt(float x, float y, float z)
{
	Vector3<float> chunkPos(floor(x / CHUNK_SIZE_X), 0, floor(z / CHUNK_SIZE_Z));
	Chunk * chunk = ChunkAt(chunkPos.x, chunkPos.z);

	if (chunk)
		return chunk->GetBlock(x - (chunkPos.x * CHUNK_SIZE_X), y, z - (chunkPos.z * CHUNK_SIZE_X));

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
							file << (int)ChunkAt(i, j)->GetBlock(x, y, z) << " ";


				std::cout << "Chunk " << count++ << " / " << total << " saved" << std::endl;
				file << std::endl;
			}
		}
	file << "END " << std::endl;
	file.close();

	std::cout << "Map saved as " << filename << std::endl << std::endl;
}

void World::AddMineral(BlockType mineral, Chunk * &chunk, int x, int y, int z)
{

	chunk->SetBlock(x, y, z, mineral);
	if (rand() % 100 >= 60 && chunk->GetBlock(x + 1, y, z) == BTYPE_STONE)
	{
		chunk->SetBlock(x + 1, y, z, mineral);
	}
	if (rand() % 100 >= 60 && chunk->GetBlock(x - 1, y, z) == BTYPE_STONE)
	{
		chunk->SetBlock(x - 1, y, z, mineral);
	}
	if (rand() % 100 >= 60 && chunk->GetBlock(x, y + 1, z) == BTYPE_STONE)
	{
		chunk->SetBlock(x, y + 1, z, mineral);
	}
	if (rand() % 100 >= 60 && chunk->GetBlock(x, y - 1, z) == BTYPE_STONE)
	{
		chunk->SetBlock(x, y - 1, z, mineral);
	}
	if (rand() % 100 >= 60 && chunk->GetBlock(x, y, z + 1) == BTYPE_STONE)
	{
		chunk->SetBlock(x, y, z + 1, mineral);
	}
	if (rand() % 100 >= 60 && chunk->GetBlock(x, y, z - 1) == BTYPE_STONE)
	{
		chunk->SetBlock(x, y, z - 1, mineral);
	}
}

void World::AddTree(Chunk * &chunk, int x, int y, int z)
{
	int hauteur = rand() % 7 + 3;

	for (int k = 0; k < hauteur; k++)
	{
		chunk->SetBlock(x, y + k, z, BTYPE_WOOD);
	}


	chunk->SetBlock(x + 1, y + hauteur - 1, z, BTYPE_LEAVE);
	chunk->SetBlock(x + 1, y + hauteur - 1, z + 1, BTYPE_LEAVE);
	chunk->SetBlock(x, y + hauteur - 1, z + 1, BTYPE_LEAVE);
	chunk->SetBlock(x, y + hauteur - 1, z - 1, BTYPE_LEAVE);
	chunk->SetBlock(x - 1, y + hauteur - 1, z - 1, BTYPE_LEAVE);
	chunk->SetBlock(x - 1, y + hauteur - 1, z, BTYPE_LEAVE);
	chunk->SetBlock(x - 1, y + hauteur - 1, z + 1, BTYPE_LEAVE);
	chunk->SetBlock(x + 1, y + hauteur - 1, z - 1, BTYPE_LEAVE);

	chunk->SetBlock(x + 1, y + hauteur, z, BTYPE_LEAVE);
	chunk->SetBlock(x, y + hauteur, z + 1, BTYPE_LEAVE);
	chunk->SetBlock(x, y + hauteur, z - 1, BTYPE_LEAVE);
	chunk->SetBlock(x - 1, y + hauteur, z, BTYPE_LEAVE);
	chunk->SetBlock(x, y + hauteur, z, BTYPE_LEAVE);
	chunk->SetBlock(x, y + hauteur + 1, z, BTYPE_LEAVE);
}

void World::InitChunks(int CenterX, int CenterZ)
{
	//Init les blocks
	for (int i = 0; i < RENDER_DISTANCE * 2; i++)
		for (int j = 0; j < RENDER_DISTANCE * 2; j++)
		{
			Chunk * chunk = ChunkAt(CenterX + i - RENDER_DISTANCE, CenterZ + j - RENDER_DISTANCE);

			//Si n'est pas creer
			if (chunk && !chunk->m_iscreated)
				InitChunk(CenterX + i - RENDER_DISTANCE, CenterZ + j - RENDER_DISTANCE);

		}
}

void World::Update(int CenterX, int CenterZ, BlockInfo* &info)
{
	Chunk * chunk = ChunkAt(CenterX, CenterZ);
	//Si dirty
	if (chunk && chunk->NeedUpdate())
		chunk->Update(info);

	//Parcours les chunk en cercle
	for (int x = 1; x < RENDER_DISTANCE; ++x)
		for (int a = 0; a <= x; ++a)
		{
			//
			chunk = ChunkAt(CenterX + a, CenterZ - x);
			if (chunk && chunk->NeedUpdate())
			{
				chunk->Update(info);
				return;
			}

			chunk = ChunkAt(CenterX - a, CenterZ - x);
			if (chunk && chunk->NeedUpdate())
			{
				chunk->Update(info);
				return;
			}

			//
			chunk = ChunkAt(CenterX + a, CenterZ + x);
			if (chunk && chunk->NeedUpdate())
			{
				chunk->Update(info);
				return;
			}

			chunk = ChunkAt(CenterX - a, CenterZ + x);
			if (chunk && chunk->NeedUpdate())
			{
				chunk->Update(info);
				return;
			}

			//
			chunk = ChunkAt(CenterX - x, CenterZ + a);
			if (chunk && chunk->NeedUpdate())
			{
				chunk->Update(info);
				return;
			}

			chunk = ChunkAt(CenterX - x, CenterZ - a);
			if (chunk && chunk->NeedUpdate())
			{
				chunk->Update(info);
				return;
			}

			//
			chunk = ChunkAt(CenterX + x, CenterZ + a);
			if (chunk && chunk->NeedUpdate())
			{
				chunk->Update(info);
				return;
			}

			chunk = ChunkAt(CenterX + x, CenterZ - a);
			if (chunk && chunk->NeedUpdate())
			{
				chunk->Update(info);
				return;
			}
		}

}

int World::ChunkNotUpdated(int CenterX, int CenterZ)
{
	int chunkNotUpdated = 0;
	for (int i = 0; i < RENDER_DISTANCE * 2; i++)
		for (int j = 0; j < RENDER_DISTANCE * 2; j++)
		{
			Chunk * chunk = ChunkAt(CenterX + i - RENDER_DISTANCE, CenterZ + j - RENDER_DISTANCE);

			if (chunk && chunk->NeedUpdate())
				chunkNotUpdated++;

		}
	return chunkNotUpdated;
}

void World::Render(int CenterX, int CenterZ, GLenum &program)
{

	//Render les blocks
	for (int i = 0; i < RENDER_DISTANCE * 2; i++)
		for (int j = 0; j < RENDER_DISTANCE * 2; j++)
		{
			Chunk * chunk = ChunkAt(CenterX + i - RENDER_DISTANCE, CenterZ + j - RENDER_DISTANCE);

			//Si le chunk existe on le render
			if (chunk)
			{
				chunk->RenderSolidBuffer(program);

			}

		}

	//Render le transparent (ex: BTYPE_WATER)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < RENDER_DISTANCE * 2; i++)
		for (int j = 0; j < RENDER_DISTANCE * 2; j++)
		{
			Chunk * chunk = ChunkAt(CenterX + i - RENDER_DISTANCE, CenterZ + j - RENDER_DISTANCE);

			//Si le chunk existe on le render
			if (chunk)
				chunk->RenderTransparentBuffer(program);
		}
	glDisable(GL_BLEND);
}
