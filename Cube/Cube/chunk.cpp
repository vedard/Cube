#include "chunk.h"
#include <iostream>
#include <sstream>
#include <string>

Chunk::Chunk() :m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_iscreated(false), m_isDirty(true), m_chunkMesh(), m_transparentMesh(), m_position(0, 0, 0), m_save(false)
{
	m_blocks.Reset(BTYPE_AIR);
	m_defaultBlock = BTYPE_AIR;

}

Chunk::~Chunk()
{

}

void Chunk::RemoveBloc(int x, int y, int z)
{
	m_blocks.Set(x, y, z, BTYPE_AIR);
	m_isDirty = true;
	m_save = true;

	// Si on efface un cube pres d'un chunk il faut le redender
	// Aussi sinon cetaine face qui ne s'afficheront pas
	if (x == 0 && m_negativeX)
		m_negativeX->m_isDirty = true;

	else if (x == CHUNK_SIZE_X - 1 && m_positiveX)
		m_positiveX->m_isDirty = true;

	if (z == 0 && m_negativeZ)
		m_negativeZ->m_isDirty = true;

	else if (z == CHUNK_SIZE_Z - 1 && m_positiveZ)
		m_positiveZ->m_isDirty = true;
}

void Chunk::SetBlock(int x, int y, int z, BlockType type, char direction)
{
	if (direction == ' ')
		direction = 'Q';
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z)
	{
		m_blocks.Set(x, y, z, type);
		m_blocks.SetDirection(x, y, z, direction);
		m_isDirty = true;

	}
	else if (x <= -1 && m_negativeX)
	{
		m_negativeX->m_blocks.Set(CHUNK_SIZE_X + x, y, z, type);
		m_negativeX->m_blocks.SetDirection(CHUNK_SIZE_X + x, y, z, direction);
		m_negativeX->m_isDirty = true;

	}
	else if (x >= CHUNK_SIZE_X && m_positiveX)
	{
		int distance = x - CHUNK_SIZE_X;
		m_positiveX->m_blocks.Set(distance, y, z, type);
		m_positiveX->m_blocks.SetDirection(distance, y, z, direction);
		m_positiveX->m_isDirty = true;

	}
	else if (z <= -1 && m_negativeZ)
	{

		m_negativeZ->m_blocks.Set(x, y, CHUNK_SIZE_Z + z, type);
		m_negativeZ->m_blocks.SetDirection(x, y, CHUNK_SIZE_Z + z, direction);
		m_negativeZ->m_isDirty = true;

	}
	else if (z >= CHUNK_SIZE_Z && m_positiveZ)
	{
		int distance = z - CHUNK_SIZE_Z;
		m_positiveZ->m_blocks.Set(x, y, distance, type);
		m_positiveZ->m_blocks.SetDirection(x, y, distance, direction);
		m_positiveZ->m_isDirty = true;

	}
}

void Chunk::PlaceBlock(int x, int y, int z, BlockType type)
{
	m_save = true;
	if (m_blocks.Get(x, y, z) == BTYPE_AIR)
		SetBlock(x, y, z, type, 'Q');


}

const BlockType& Chunk::GetBlock(int x, int y, int z) const
{
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z)
		return m_blocks.Get(x, y, z);

	else if (x <= -1 && m_negativeX)
		return m_negativeX->GetBlock(CHUNK_SIZE_X + x, y, z);

	else if (x >= CHUNK_SIZE_X && m_positiveX)
		return m_positiveX->GetBlock(x - CHUNK_SIZE_X, y, z);

	else if (z <= -1 && m_negativeZ)
		return m_negativeZ->GetBlock(x, y, CHUNK_SIZE_Z + z);

	else if (z >= CHUNK_SIZE_Z && m_positiveZ)
		return m_positiveZ->GetBlock(x, y, z - CHUNK_SIZE_Z);

	else
		return m_defaultBlock;
}

void Chunk::SetPosition(int x, int y, int z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

const Vector3<float> &Chunk::GetPosition() const
{
	return m_position;
}

void Chunk::Update(BlockInfo* &binfo)
{
	// Update mesh
	if (m_isDirty)
	{
		m_isDirty = false;
		int maxVertexCount = (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z) * (6 * 4);

		//On creer un buffer pour les solid et un autre pour les transparents 
		ChunkMesh::VertexData * vds = new ChunkMesh::VertexData[maxVertexCount];
		ChunkMesh::VertexData * vdt = new ChunkMesh::VertexData[maxVertexCount];
		int count_s = 0;
		int count_t = 0;

		//Ajoute les cubes sans transparence en premier
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
				for (int y = 0; y < CHUNK_SIZE_Y; ++y)
				{
					if (count_s > USHRT_MAX)
						break;
					if (count_t > USHRT_MAX)
						break;

					BlockType bt = GetBlock(x, y, z);

					if (bt != BTYPE_AIR && bt != BTYPE_WATER && bt != BTYPE_LEAVE)
						AddBlockToMesh(vds, count_s, binfo[bt], Vector3<float>(x + m_position.x, y + m_position.y, z + m_position.z));

					if (bt == BTYPE_WATER || bt == BTYPE_LEAVE || bt == BTYPE_RWATER1 || bt == BTYPE_RWATER2 || bt == BTYPE_RWATER3)
						AddBlockToMesh(vdt, count_t, binfo[bt], Vector3<float>(x + m_position.x, y + m_position.y, z + m_position.z));


				}
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
				for (int y = 0; y < CHUNK_SIZE_Y; ++y)
				{

					BlockType bt = GetBlock(x, y, z);
					if (bt == BTYPE_WATER || bt == BTYPE_FWATER)
					{
						AddWater(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
				}
		if (count_s > USHRT_MAX)
		{
			count_s = USHRT_MAX;
			std::cout << "[ Chunk :: Update ] Chunk data truncaned , too much vertices to have a 16 bit index " << std::endl;
		}
		if (count_t > USHRT_MAX)
		{
			count_t = USHRT_MAX;
			std::cout << "[ Chunk :: Update ] Chunk data truncaned , too much vertices to have a 16 bit index " << std::endl;
		}

		m_chunkMesh.SetMeshData(vds, count_s);
		m_transparentMesh.SetMeshData(vdt, count_t);
		delete[] vds;
		delete[] vdt;
		if (m_isDirty)
			Update(binfo);
	}
}

void Chunk::AddBlockToMesh(ChunkMesh::VertexData * &vd, int& count, BlockInfo &binfo, const Vector3<float> &Blockpos)
{
	BlockType type = binfo.GetType();
	/*if (type == BTYPE_WATER)
		CheckWater(Vector3<float>((int)Blockpos.x % CHUNK_SIZE_X, (int)Blockpos.y % CHUNK_SIZE_Y, (int)Blockpos.z % CHUNK_SIZE_Z), binfo, 0);*/

		// face
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(0, 0, -1)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(0, 0, -1));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .50f, type);

	}
	// Droite
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(1, 0, 0)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(1, 0, 0));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .25f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .25f, type);
	}
	//Gauche
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(-1, 0, 0)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(-1, 0, 0));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .25f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .25f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .50f, type);
	}
	//Derirere
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(0, 0, 1)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(0, 0, 1));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
	}
	//Haut
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(0, 1, 0)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(0, 1, 0));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * 1.0f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * 1.0f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .75f, type);

		if (type == BTYPE_WATER || BTYPE_LAVA) ///Lorsque sous l'eau on peut voir le haut de locean
		{
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .75f, type);
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * 1.0f, type);
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 1.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * 1.0f, type);

		}
	}
	//Bas
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(0, -1, 0)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(0, -1, 0));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * 1.0f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * 1.0f, type);

	}

}

void Chunk::RenderSolidBuffer(GLenum &m_program) const
{
	m_chunkMesh.Render(m_program);
}

void Chunk::RenderTransparentBuffer(GLenum &m_program) const
{
	m_transparentMesh.Render(m_program);
}

void Chunk::DeleteCache()
{
	m_chunkMesh.DeleteBuffer();
	m_transparentMesh.DeleteBuffer();
	m_isDirty = true;
}

bool Chunk::NeedUpdate() const
{
	//Si le chunk est dirty et que tous ses voisin on été creer
	if (m_isDirty
		&& (m_negativeX == NULL || m_negativeX->m_iscreated)
		&& (m_negativeZ == NULL || m_negativeZ->m_iscreated)
		&& (m_positiveX == NULL || m_positiveX->m_iscreated)
		&& (m_positiveZ == NULL || m_positiveZ->m_iscreated))
		return m_isDirty;

	else
		return false;
}

bool& Chunk::GetSave() {
	return m_save;
}

bool Chunk::CheckFace(BlockType type, const Vector3<float> &Blockpos, const Vector3<float> &face) const
{
	BlockType faceType = GetBlock(Blockpos.x + face.x, Blockpos.y + face.y, Blockpos.z + face.z);


	if (faceType == BTYPE_AIR || faceType == BTYPE_LEAVE || (faceType != BTYPE_WATER && type == BTYPE_WATER) || (faceType != BTYPE_LAVA&& type == BTYPE_LAVA) || (faceType == BTYPE_LAVA&& type != BTYPE_LAVA) || (faceType == BTYPE_WATER && type != BTYPE_WATER))
		return true;

	return false;
}

float Chunk::CheckLightning(const Vector3<float> &Blockpos, const Vector3<float> &face) const
{

	if (face.y == -1)
		return 0.5f;

	for (int i = 1; true; i++)
	{
		BlockType block = GetBlock(Blockpos.x + face.x, Blockpos.y + i, Blockpos.z + face.z);
		if (block == BTYPE_AIR)
		{
			if (Blockpos.y + i > CHUNK_SIZE_Y)
				return 0.9f;

		}
		else if (block == BTYPE_LEAVE)
			return 0.74f;
		else if (block == BTYPE_WATER)
			return 0.64f;
		else
			return  0.57f;
	}
}


//obsolete garder pour référence

BlockType Chunk::ChooseWater(int numero)
{
	if (numero == 1)
		return BTYPE_RWATER1;
	else if (numero == 2)
		return BTYPE_RWATER2;
	else
		return BTYPE_RWATER3;
}

const char& Chunk::GetDirection(int x, int y, int z) const
{
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z)
		return m_blocks.GetDirection(x, y, z);

	else if (x >= -1 && m_negativeX)
		return m_negativeX->GetDirection(CHUNK_SIZE_X + x, y, z);

	else if (x <= CHUNK_SIZE_X && m_positiveX)
		return m_positiveX->GetDirection(x - CHUNK_SIZE_X - 1, y, z);

	else if (z <= -1 && m_negativeZ)
		return m_negativeZ->GetDirection(x, y, CHUNK_SIZE_Z + z);

	else if (z >= CHUNK_SIZE_Z && m_positiveZ)
		return m_positiveZ->GetDirection(x, y, z - CHUNK_SIZE_Z - 1);

	else
		return 'Q';
}


//Fait couler l'eau
void Chunk::AddWater(const Vector3<float> &Blockpos)
{
	BlockType blockwater[3] = { BTYPE_RWATER1, BTYPE_RWATER2, BTYPE_RWATER3 };
	BlockType bt = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z);
	if (bt != BTYPE_WATER && bt != BTYPE_FWATER)
	{

		char direction = GetDirection(Blockpos.x, Blockpos.y, Blockpos.z);
		if (direction == 'Q')
		{
			//TODO change
			direction = GetDirection(Blockpos);
			m_blocks.SetDirection(Blockpos.x, Blockpos.y, Blockpos.z, direction);
		}
		if (direction != 'Q')
		{
			int xModif = 0;
			int zModif = 0;
			switch (direction)
			{
			case 'N':
				xModif = 0;
				zModif = 1;
				break;
			case 'E':
				xModif = 1;
				zModif = 0;
				break;
			case 'W':
				xModif = -1;
				zModif = 0;
				break;
			case 'S':
				xModif = 0;
				zModif = -1;
				break;
			default:
				xModif = 0;
				zModif = 0;
				break;
			}
			bool bcontinue = true;
			if (bt == BTYPE_AIR)
			{
				bcontinue = false;
				SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z, BTYPE_FWATER, direction);
			}

			bool bExploser = true;
			for (int i = 1; i < 4; i++)
			{
				BlockType blockkk = GetBlock(Blockpos.x + (i * xModif), Blockpos.y - 1, Blockpos.z + (i * zModif));
				if (blockkk == BTYPE_AIR || blockkk == BTYPE_RWATER1 || blockkk == BTYPE_RWATER2 || blockkk == BTYPE_RWATER3 || blockkk == BTYPE_WATER || blockkk == BTYPE_FWATER)
				{
					bExploser = false;
					break;
				}
			}
			if (!bExploser)
			{

				for (int i = 1; i < 4; i++)
				{
					if (bcontinue)
					{
						BlockType waterType = ChooseWater(i);
						BlockType blockSuivant = GetBlock(Blockpos.x + (i * xModif), Blockpos.y, Blockpos.z + (i * zModif));
						if (blockSuivant == BTYPE_AIR)// || blockSuivant == BTYPE_RWATER1 || blockSuivant == BTYPE_RWATER2 || blockSuivant == BTYPE_RWATER3)
						{

							SetBlock(Blockpos.x + (i * xModif), Blockpos.y, Blockpos.z + (i * zModif), waterType, direction);
							BlockType blocDessous = GetBlock(Blockpos.x + (i * xModif), Blockpos.y - 1, Blockpos.z + (i * zModif));
							if (blocDessous == BTYPE_WATER || blocDessous == BTYPE_FWATER)
								bcontinue = false;
							else if (blocDessous == BTYPE_AIR)
							{
								SetBlock(Blockpos.x + (i * xModif), Blockpos.y - 1, Blockpos.z + (i * zModif), BTYPE_FWATER, direction);
								bcontinue = false;
							}
						}
						else
							bcontinue = false;
					}
				}
			}
			else
				WaterExplosion(Blockpos);
		}
	}

}

//Retourne la direction que devrait prendre l'eau
char Chunk::GetDirection(const Vector3<float> &Blockpos)
{
	int North = 0, South = 0, East = 0, West = 0;
	int i = 1;
	if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1) == BTYPE_WATER && GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z) == BTYPE_WATER &&
		GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1) == BTYPE_WATER && GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z) == BTYPE_WATER)
		return 'Q';
	do
	{
		if (GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z + (i)) == BTYPE_AIR)
			North = i;

		if (i >= 4)
			North = i;

		i++;
	} while (North == 0);
	i = 0;
	do
	{
		if (GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z - (i)) == BTYPE_AIR)
			South = i;
		if (i >= 4)
			South = i;
		i++;

	} while (South == 0);
	i = 0;
	do
	{
		if (GetBlock(Blockpos.x - i, Blockpos.y - 1, Blockpos.z) == BTYPE_AIR)
			West = i;

		if (i >= 4)
			West = i;
		i++;

	} while (West == 0);
	i = 0;
	do
	{
		if (GetBlock(Blockpos.x + i, Blockpos.y - 1, Blockpos.z) == BTYPE_AIR)
			East = i;

		if (i >= 4)
			East = i;

		i++;
	} while (East == 0);

	std::string direction;
	int distance;
	if (North == South)
	{
		direction = "NS";
		distance = North;
	}
	else if (North > South)
	{
		direction = "S";
		distance = South;
	}
	else
	{
		direction = "N";
		distance = North;
	}

	if (distance == East)
		direction += "E";
	else if (distance > East)
	{
		direction = "E";
		distance = East;
	}

	if (distance == West)
		direction += "W";
	else if (distance > West)
	{
		direction = "W";
		distance = West;
	}

	if (direction.length() > 1)
	{
		srand(time(NULL));
		return direction[rand() % direction.length()];
	}
	else return direction[0];

}

//Faire l'explosion de l'eau
void Chunk::WaterExplosion(const Vector3<float> &Blockpos)
{
	BlockType BlocktoCheck;
#pragma region nord 
	//Complet
	BlocktoCheck = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1);
	if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER1)
	{
		if (GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z + 1) == BTYPE_AIR)
			SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z + 1, BTYPE_FWATER, 'N');

		else
		{
			if (BlocktoCheck != BTYPE_RWATER1)
				SetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1, BTYPE_RWATER1, 'N');

			//GAUCHE
			BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z + 1);
			if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER2)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z + 1, BTYPE_RWATER2, 'N');

				BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z + 1);
				if (BlocktoCheck == BTYPE_AIR)
					SetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z + 1, BTYPE_FWATER, 'N');
				else if (BlocktoCheck != BTYPE_WATER && BlocktoCheck != BTYPE_FWATER)
				{
					BlocktoCheck = GetBlock(Blockpos.x + 2, Blockpos.y, Blockpos.z + 1);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x + 2, Blockpos.y, Blockpos.z + 1, BTYPE_RWATER3, 'E');
						if (GetBlock(Blockpos.x + 2, Blockpos.y - 1, Blockpos.z + 1) == BTYPE_AIR)
							SetBlock(Blockpos.x + 2, Blockpos.y - 1, Blockpos.z + 1, BTYPE_FWATER, 'E');
					}
					BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z + 2);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z + 2, BTYPE_RWATER3, 'N');
						if (GetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z + 2) == BTYPE_AIR)
							SetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z + 2, BTYPE_FWATER, 'N');
					}
				}
			}

			//CENTRE
			BlocktoCheck = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 2);
			if (BlocktoCheck == BTYPE_AIR || BTYPE_RWATER3)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 2, BTYPE_RWATER2, 'N');
				if (GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z + 2) == BTYPE_AIR)
					SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z + 2, BTYPE_FWATER, 'N');
				else if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 3) == BTYPE_AIR)
				{
					SetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 3, BTYPE_RWATER3, 'N');
					if (GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z + 3) == BTYPE_AIR)
						SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z + 3, BTYPE_FWATER, 'N');
				}
			}

			//DROITE
			BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z + 1);
			if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER2)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z + 1, BTYPE_RWATER2, 'N');

				BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z + 1);
				if (BlocktoCheck == BTYPE_AIR)
					SetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z + 1, BTYPE_FWATER, 'N');
				else if (BlocktoCheck != BTYPE_WATER && BlocktoCheck != BTYPE_FWATER)
				{
					BlocktoCheck = GetBlock(Blockpos.x - 2, Blockpos.y, Blockpos.z + 1);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x - 2, Blockpos.y, Blockpos.z + 1, BTYPE_RWATER3, 'W');
						if (GetBlock(Blockpos.x - 2, Blockpos.y - 1, Blockpos.z + 1) == BTYPE_AIR)
							SetBlock(Blockpos.x - 2, Blockpos.y - 1, Blockpos.z + 1, BTYPE_FWATER, 'W');
					}
					BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z + 2);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z + 2, BTYPE_RWATER3, 'N');
						if (GetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z + 2) == BTYPE_AIR)
							SetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z + 2, BTYPE_FWATER, 'N');
					}
				}
			}
		}
	}
#pragma endregion
#pragma region Sud
	BlocktoCheck = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1);
	if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER1)
	{
		if (GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z - 1) == BTYPE_AIR)
			SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z - 1, BTYPE_FWATER, 'S');

		else
		{
			if (BlocktoCheck != BTYPE_RWATER1)
				SetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1, BTYPE_RWATER1, 'S');

			//GAUCHE
			BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z - 1);
			if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER2)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z - 1, BTYPE_RWATER2, 'S');

				BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z - 1);
				if (BlocktoCheck == BTYPE_AIR)
					SetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z - 1, BTYPE_FWATER, 'S');
				else if (BlocktoCheck != BTYPE_WATER && BlocktoCheck != BTYPE_FWATER)
				{
					BlocktoCheck = GetBlock(Blockpos.x + 2, Blockpos.y, Blockpos.z - 1);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x + 2, Blockpos.y, Blockpos.z - 1, BTYPE_RWATER3, 'E');
						if (GetBlock(Blockpos.x + 2, Blockpos.y - 1, Blockpos.z - 1) == BTYPE_AIR)
							SetBlock(Blockpos.x + 2, Blockpos.y - 1, Blockpos.z - 1, BTYPE_FWATER, 'E');
					}
					BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z - 2);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z - 2, BTYPE_RWATER3, 'S');
						if (GetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z - 2) == BTYPE_AIR)
							SetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z - 2, BTYPE_FWATER, 'S');
					}
				}
			}

			//CENTRE
			BlocktoCheck = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 2);
			if (BlocktoCheck == BTYPE_AIR || BTYPE_RWATER3)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 2, BTYPE_RWATER2, 'S');
				if (GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z - 2) == BTYPE_AIR)
					SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z - 2, BTYPE_FWATER, 'S');
				else if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 3) == BTYPE_AIR)
				{
					SetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 3, BTYPE_RWATER3, 'S');
					if (GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z - 3) == BTYPE_AIR)
						SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z - 3, BTYPE_FWATER, 'S');

				}

			}

			//DROITE
			BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z - 1);
			if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER2)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z - 1, BTYPE_RWATER2, 'S');

				BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z - 1);
				if (BlocktoCheck == BTYPE_AIR)
					SetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z - 1, BTYPE_FWATER, 'S');
				else if (BlocktoCheck != BTYPE_WATER && BlocktoCheck != BTYPE_FWATER)
				{
					BlocktoCheck = GetBlock(Blockpos.x - 2, Blockpos.y, Blockpos.z - 1);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x - 2, Blockpos.y, Blockpos.z - 1, BTYPE_RWATER3, 'W');
						if (GetBlock(Blockpos.x - 2, Blockpos.y - 1, Blockpos.z - 1) == BTYPE_AIR)
							SetBlock(Blockpos.x - 2, Blockpos.y - 1, Blockpos.z - 1, BTYPE_FWATER, 'W');
					}
					BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z - 2);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z - 2, BTYPE_RWATER3, 'S');
						if (GetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z - 2) == BTYPE_AIR)
							SetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z - 2, BTYPE_FWATER, 'S');
					}
				}
			}
		}
	}
#pragma endregion
#pragma region West
	BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z);
	if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER1)
	{
		if (GetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z) == BTYPE_AIR)
			SetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z, BTYPE_FWATER, 'W');
		else {
			if (BlocktoCheck != BTYPE_RWATER1)
				SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z, BTYPE_RWATER1, 'W');

			//GAUCHE
			BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z + 1);
			if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER2)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z + 1, BTYPE_RWATER2, 'W');

				BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z + 1);
				if (BlocktoCheck == BTYPE_AIR)
					SetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z + 1, BTYPE_FWATER, 'W');
				else if (BlocktoCheck != BTYPE_WATER && BlocktoCheck != BTYPE_FWATER)
				{
					BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z + 2);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z + 2, BTYPE_RWATER3, 'N');
						if (GetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z + 2) == BTYPE_AIR)
							SetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z + 2, BTYPE_FWATER, 'N');
					}
					BlocktoCheck = GetBlock(Blockpos.x - 2, Blockpos.y, Blockpos.z + 1);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x - 2, Blockpos.y, Blockpos.z + 1, BTYPE_RWATER3, 'W');
						if (GetBlock(Blockpos.x - 2, Blockpos.y - 1, Blockpos.z + 1) == BTYPE_AIR)
							SetBlock(Blockpos.x - 2, Blockpos.y - 1, Blockpos.z + 1, BTYPE_FWATER, 'W');
					}
				}
			}

			//CENTRE
			BlocktoCheck = GetBlock(Blockpos.x - 2, Blockpos.y, Blockpos.z);
			if (BlocktoCheck == BTYPE_AIR || BTYPE_RWATER3)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x - 2, Blockpos.y, Blockpos.z, BTYPE_RWATER2, 'W');
				if (GetBlock(Blockpos.x - 2, Blockpos.y - 1, Blockpos.z) == BTYPE_AIR)
					SetBlock(Blockpos.x - 2, Blockpos.y - 1, Blockpos.z, BTYPE_FWATER, 'W');
				else if (GetBlock(Blockpos.x - 3, Blockpos.y, Blockpos.z) == BTYPE_AIR)
				{
					SetBlock(Blockpos.x - 3, Blockpos.y, Blockpos.z, BTYPE_RWATER3, 'W');
					if (GetBlock(Blockpos.x - 3, Blockpos.y - 1, Blockpos.z) == BTYPE_AIR)
						SetBlock(Blockpos.x - 3, Blockpos.y - 1, Blockpos.z, BTYPE_FWATER, 'W');
				}

			}

			//DROITE
			BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z - 1);
			if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER2)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z - 1, BTYPE_RWATER2, 'W');

				BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z - 1);
				if (BlocktoCheck == BTYPE_AIR)
					SetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z - 1, BTYPE_FWATER, 'W');
				else if (BlocktoCheck != BTYPE_WATER && BlocktoCheck != BTYPE_FWATER)
				{
					BlocktoCheck = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z - 2);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z - 2, BTYPE_RWATER3, 'S');
						if (GetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z - 2) == BTYPE_AIR)
							SetBlock(Blockpos.x - 1, Blockpos.y - 1, Blockpos.z - 2, BTYPE_FWATER, 'S');
					}
					BlocktoCheck = GetBlock(Blockpos.x - 2, Blockpos.y, Blockpos.z - 1);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x - 2, Blockpos.y, Blockpos.z - 1, BTYPE_RWATER3, 'W');
						if (GetBlock(Blockpos.x - 2, Blockpos.y - 1, Blockpos.z - 1) == BTYPE_AIR)
							SetBlock(Blockpos.x - 2, Blockpos.y - 1, Blockpos.z - 1, BTYPE_FWATER, 'W');
					}
				}
			}
		}
	}

#pragma endregion
#pragma region East
	BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z);
	if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER1)
	{
		if (GetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z) == BTYPE_AIR)
			SetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z, BTYPE_FWATER, 'E');
		else
		{
			if (BlocktoCheck != BTYPE_RWATER1)
				SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z, BTYPE_RWATER1, 'E');

			//GAUCHE
			BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z + 1);
			if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER2)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z + 1, BTYPE_RWATER2, 'E');

				BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z + 1);
				if (BlocktoCheck == BTYPE_AIR)
					SetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z + 1, BTYPE_FWATER, 'E');
				else if (BlocktoCheck != BTYPE_WATER && BlocktoCheck != BTYPE_FWATER)
				{
					BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z + 2);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z + 2, BTYPE_RWATER3, 'N');
						if (GetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z + 2) == BTYPE_AIR)
							SetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z + 2, BTYPE_FWATER, 'N');
					}
					BlocktoCheck = GetBlock(Blockpos.x + 2, Blockpos.y, Blockpos.z + 1);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x + 2, Blockpos.y, Blockpos.z + 1, BTYPE_RWATER3, 'E');
						if (GetBlock(Blockpos.x + 2, Blockpos.y - 1, Blockpos.z + 1) == BTYPE_AIR)
							SetBlock(Blockpos.x + 2, Blockpos.y - 1, Blockpos.z + 1, BTYPE_FWATER, 'E');
					}
				}
			}

			//CENTRE
			BlocktoCheck = GetBlock(Blockpos.x + 2, Blockpos.y, Blockpos.z);
			if (BlocktoCheck == BTYPE_AIR || BTYPE_RWATER3)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x + 2, Blockpos.y, Blockpos.z, BTYPE_RWATER2, 'E');
				if (GetBlock(Blockpos.x + 2, Blockpos.y - 1, Blockpos.z) == BTYPE_AIR)
					SetBlock(Blockpos.x + 2, Blockpos.y - 1, Blockpos.z, BTYPE_FWATER, 'E');
				else if (GetBlock(Blockpos.x + 3, Blockpos.y, Blockpos.z) == BTYPE_AIR)
				{
					SetBlock(Blockpos.x + 3, Blockpos.y, Blockpos.z, BTYPE_RWATER3, 'E');
					if (GetBlock(Blockpos.x + 3, Blockpos.y - 1, Blockpos.z) == BTYPE_AIR)
						SetBlock(Blockpos.x + 3, Blockpos.y - 1, Blockpos.z, BTYPE_FWATER, 'E');
				}

			}

			//DROITE
			BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z - 1);
			if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER2)
			{
				if (BlocktoCheck != BTYPE_RWATER2)
					SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z - 1, BTYPE_RWATER2, 'E');

				BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z - 1);
				if (BlocktoCheck == BTYPE_AIR)
					SetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z - 1, BTYPE_FWATER, 'E');
				else if (BlocktoCheck != BTYPE_WATER && BlocktoCheck != BTYPE_FWATER)
				{
					BlocktoCheck = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z - 2);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z - 2, BTYPE_RWATER3, 'S');
						if (GetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z - 2) == BTYPE_AIR)
							SetBlock(Blockpos.x + 1, Blockpos.y - 1, Blockpos.z - 2, BTYPE_FWATER, 'S');
					}
					BlocktoCheck = GetBlock(Blockpos.x + 2, Blockpos.y, Blockpos.z - 1);
					if (BlocktoCheck == BTYPE_AIR || BlocktoCheck == BTYPE_RWATER3)
					{
						if (BlocktoCheck != BTYPE_RWATER3)
							SetBlock(Blockpos.x + 2, Blockpos.y, Blockpos.z - 1, BTYPE_RWATER3, 'E');
						if (GetBlock(Blockpos.x + 2, Blockpos.y - 1, Blockpos.z - 1) == BTYPE_AIR)
							SetBlock(Blockpos.x + 2, Blockpos.y - 1, Blockpos.z - 1, BTYPE_FWATER, 'E');
					}
				}
			}
		}

	}
#pragma endregion


}




