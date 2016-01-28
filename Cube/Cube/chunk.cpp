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
	m_position.x = (float)x;
	m_position.y = (float)y;
	m_position.z = (float)z;
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

		//Delete when timer/thread works
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
				for (int y = 0; y < CHUNK_SIZE_Y; ++y)
				{
					BlockType bt = GetBlock(x, y, z);

					if (bt == BTYPE_WATER || bt == BTYPE_FWATER)// && TickCount == 0)
					{
						Water1(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
					if (bt == BTYPE_RWATER1)// && TickCount == 1)
					{
						Water2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
					if (bt == BTYPE_RWATER2)// && TickCount == 2)
					{
						Water2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
					if (bt == BTYPE_RWATER3)// && TickCount == 3)
					{
						Water2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
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
	float h = binfo.GetHauteur();
	/*if (type == BTYPE_WATER)
		CheckWater(Vector3<float>((int)Blockpos.x % CHUNK_SIZE_X, (int)Blockpos.y % CHUNK_SIZE_Y, (int)Blockpos.z % CHUNK_SIZE_Z), binfo, 0);*/

		// face
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(0, 0, -1)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(0, 0, -1));
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + h, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + h, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .50f, type);

	}
	// Droite
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(1, 0, 0)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(1, 0, 0));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .25f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + h, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + h, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .25f, type);
	}
	//Gauche
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(-1, 0, 0)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(-1, 0, 0));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .25f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .25f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + h, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + h, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .50f, type);
	}
	//Derirere
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(0, 0, 1)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(0, 0, 1));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + 0.f, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .50f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + h, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + h, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
	}
	//Haut
	if (CheckFace(type, Blockpos - m_position, Vector3<float>(0, 1, 0)))
	{
		float light = CheckLightning(Blockpos - m_position, Vector3<float>(0, 1, 0));

		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + h, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + h, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * 1.0f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + h, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * 1.0f, type);
		vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + h, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .75f, type);

		if (type == BTYPE_WATER) ///Lorsque sous l'eau on peut voir le haut de locean
		{
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + h, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f, type);
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + h, Blockpos.z + 0.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .75f, type);
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 1.f, Blockpos.y + h, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .00f, binfo.v + binfo.h * 1.0f, type);
			vd[count++] = ChunkMesh::VertexData(Blockpos.x + 0.f, Blockpos.y + h, Blockpos.z + 1.f, light, light, light, binfo.u + binfo.w * .50f, binfo.v + binfo.h * 1.0f, type);

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


	if (faceType == BTYPE_AIR || BTYPE_RWATER1 == faceType || faceType == BTYPE_RWATER2 || faceType == BTYPE_RWATER3
		|| faceType == BTYPE_FWATER || faceType == BTYPE_LEAVE || (faceType != BTYPE_WATER && type == BTYPE_WATER)
		|| faceType == BTYPE_LAVA || (faceType == BTYPE_WATER && type != BTYPE_WATER))
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


void Chunk::SetExploded(int x, int y, int z, bool exploded)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z)
	{
		m_blocks.SetExploded(x, y, z, exploded);
	}
	else if (x <= -1 && m_negativeX)
	{
		m_negativeX->m_blocks.SetExploded(CHUNK_SIZE_X + x, y, z, exploded);
	}
	else if (x >= CHUNK_SIZE_X && m_positiveX)
	{
		int distance = x - CHUNK_SIZE_X;
		m_positiveX->m_blocks.SetExploded(distance, y, z, exploded);
	}
	else if (z <= -1 && m_negativeZ)
	{
		m_negativeZ->m_blocks.SetExploded(x, y, CHUNK_SIZE_Z + z, exploded);

	}
	else if (z >= CHUNK_SIZE_Z && m_positiveZ)
	{
		int distance = z - CHUNK_SIZE_Z;
		m_positiveZ->m_blocks.SetExploded(x, y, distance, exploded);
	}
}
bool Chunk::GetExploded(int x, int y, int z) const
{
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z)
		return m_blocks.GetExploded(x, y, z);

	else if (x >= -1 && m_negativeX)
		return m_negativeX->GetExploded(CHUNK_SIZE_X + x, y, z);

	else if (x <= CHUNK_SIZE_X && m_positiveX)
		return m_positiveX->GetExploded(x - CHUNK_SIZE_X - 1, y, z);

	else if (z <= -1 && m_negativeZ)
		return m_negativeZ->GetExploded(x, y, CHUNK_SIZE_Z + z);

	else if (z >= CHUNK_SIZE_Z && m_positiveZ)
		return m_positiveZ->GetExploded(x, y, z - CHUNK_SIZE_Z - 1);

	else
		return false;

}

void Chunk::Water1(const Vector3<float> &Blockpos)
{
	BlockType bt = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z);
	if (bt != BTYPE_FWATER)
	{
		char direction = GetDirection(Blockpos.x, Blockpos.y, Blockpos.z);
		if (bt == BTYPE_AIR)
		{
			SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z, BTYPE_FWATER, direction);
			SetSource(Blockpos.x, Blockpos.y, Blockpos.z,Vector3<float>(Blockpos.x + m_position.x, Blockpos.y + m_position.y, Blockpos.z + m_position.z));
		}
		else
		{
			if (direction == 'Q')
			{
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

				if (GetExploded(Blockpos.x, Blockpos.y, Blockpos.z) == false)
				{
					bool bExplode = true;
					for (int i = 1; i < 4; i++)
					{
						BlockType blockkk = GetBlock(Blockpos.x + (i * xModif), Blockpos.y - 1, Blockpos.z + (i * zModif));
						if (blockkk == BTYPE_AIR || blockkk == BTYPE_RWATER1 || blockkk == BTYPE_RWATER2 || blockkk == BTYPE_RWATER3 || blockkk == BTYPE_WATER || blockkk == BTYPE_FWATER)
						{
							bExplode = false;
							break;
						}
					}
					if (bExplode == true)
					{
						SetExploded(Blockpos.x, Blockpos.y, Blockpos.z, true);
						if (GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z) == BTYPE_AIR)
						{
							SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z, BTYPE_RWATER1, direction);
							SetExploded(Blockpos.x + 1, Blockpos.y, Blockpos.z, true);
							SetSource(Blockpos.x + 1, Blockpos.y, Blockpos.z, Vector3<float>(Blockpos.x + m_position.x, Blockpos.y + m_position.y, Blockpos.z + m_position.z));
						}
						if (GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z) == BTYPE_AIR)
						{
							SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z, BTYPE_RWATER1, direction);
							SetExploded(Blockpos.x - 1, Blockpos.y, Blockpos.z, true);
							SetSource(Blockpos.x - 1, Blockpos.y, Blockpos.z, Vector3<float>(Blockpos.x + m_position.x, Blockpos.y + m_position.y, Blockpos.z + m_position.z));
						}
						if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1) == BTYPE_AIR)
						{
							SetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1, BTYPE_RWATER1, direction);
							SetExploded(Blockpos.x, Blockpos.y, Blockpos.z + 1, true);
							SetSource(Blockpos.x, Blockpos.y, Blockpos.z + 1, Vector3<float>(Blockpos.x + m_position.x, Blockpos.y + m_position.y, Blockpos.z + m_position.z));

						}
						if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1) == BTYPE_AIR)
						{
							SetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1, BTYPE_RWATER1, direction);
							SetExploded(Blockpos.x, Blockpos.y, Blockpos.z - 1, true);
							SetSource(Blockpos.x, Blockpos.y, Blockpos.z - 1, Vector3<float>(Blockpos.x + m_position.x, Blockpos.y + m_position.y, Blockpos.z + m_position.z));
						}
					}
					else
					{
						if (GetBlock(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif) == BTYPE_AIR)
						{
							SetBlock(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif, BTYPE_RWATER1, direction);
							SetSource(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif, Vector3<float>(Blockpos.x + m_position.x, Blockpos.y + m_position.y, Blockpos.z + m_position.z));
						}
					}
				}
			}
		}
	}
}

void Chunk::Water2(const Vector3<float> &Blockpos)
{
	if (GetExploded(Blockpos.x, Blockpos.y, Blockpos.z) == true)
		WaterExploded(Blockpos);
	else
	{
		char direction = GetDirection(Blockpos.x, Blockpos.y, Blockpos.z);
		BlockType bt = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z);
		if (bt == BTYPE_AIR || bt == BTYPE_RWATER1 || bt == BTYPE_RWATER2 || bt == BTYPE_RWATER3)
		{
			SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z, BTYPE_FWATER, direction);
			SetSource(Blockpos.x, Blockpos.y - 1, Blockpos.z, GetSource(Blockpos.x, Blockpos.y, Blockpos.z));
		}
		else if (bt != BTYPE_FWATER && bt != BTYPE_WATER)
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
			BlockType blockToAdd = WaterCheck(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif, GetBlock(Blockpos.x, Blockpos.y, Blockpos.z));
			if (blockToAdd != BTYPE_AIR)
			{
				if (GetBlock(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif) == BTYPE_AIR)
				{
					SetBlock(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif, blockToAdd, direction);
					SetSource(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif, GetSource(Blockpos.x, Blockpos.y, Blockpos.z));

				}
			}
		}
	}
}

void Chunk::WaterExploded(const Vector3<float> &Blockpos)
{
	BlockType blockUnder = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z);
	if (blockUnder == BTYPE_AIR || blockUnder == BTYPE_RWATER1 || blockUnder == BTYPE_RWATER2 || blockUnder == BTYPE_RWATER3)
	{
		SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z, BTYPE_FWATER, GetDirection(Blockpos.x, Blockpos.y, Blockpos.z));
		SetExploded(Blockpos.x, Blockpos.y - 1, Blockpos.z, false);
		SetSource(Blockpos.x, Blockpos.y - 1, Blockpos.z, GetSource(Blockpos.x, Blockpos.y, Blockpos.z));
	}
	else if (blockUnder != BTYPE_FWATER && blockUnder != BTYPE_WATER)
	{
		BlockType bt = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z);
		BlockType blockToAdd = WaterCheck(Blockpos.x, Blockpos.y, Blockpos.z + 1, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1) == BTYPE_AIR)
			{
				SetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1, blockToAdd, 'N');
				SetExploded(Blockpos.x, Blockpos.y, Blockpos.z + 1, true);
				SetSource(Blockpos.x, Blockpos.y, Blockpos.z + 1, GetSource(Blockpos.x, Blockpos.y, Blockpos.z));
			}
		}
		blockToAdd = WaterCheck(Blockpos.x, Blockpos.y, Blockpos.z - 1, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1) == BTYPE_AIR)
			{
				SetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1, blockToAdd, 'S');
				SetExploded(Blockpos.x, Blockpos.y, Blockpos.z - 1, true);
				SetSource(Blockpos.x, Blockpos.y, Blockpos.z - 1, GetSource(Blockpos.x, Blockpos.y, Blockpos.z));
			}
		}
		blockToAdd = WaterCheck(Blockpos.x + 1, Blockpos.y, Blockpos.z, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			if (GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z) == BTYPE_AIR)
			{
				SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z, blockToAdd, 'E');
				SetExploded(Blockpos.x + 1, Blockpos.y, Blockpos.z, true);
				SetSource(Blockpos.x + 1, Blockpos.y, Blockpos.z, GetSource(Blockpos.x, Blockpos.y, Blockpos.z));
			}
		}
		blockToAdd = WaterCheck(Blockpos.x - 1, Blockpos.y, Blockpos.z, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			if (GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z) == BTYPE_AIR)
			{
				SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z, blockToAdd, 'W');
				SetExploded(Blockpos.x - 1, Blockpos.y, Blockpos.z, true);
				SetSource(Blockpos.x - 1, Blockpos.y, Blockpos.z, GetSource(Blockpos.x, Blockpos.y, Blockpos.z));
			}
		}
	}
}

BlockType Chunk::WaterCheck(int x, int y, int z, BlockType bt)
{
	BlockType BlockToCheck = GetBlock(x, y, z);
	if (bt == BTYPE_FWATER && (BlockToCheck == BTYPE_RWATER1 || BlockToCheck == BTYPE_RWATER2 || BlockToCheck == BTYPE_RWATER3 || BlockToCheck == BTYPE_AIR))
		return BTYPE_RWATER1;
	else if (bt == BTYPE_WATER && (BlockToCheck == BTYPE_RWATER1 || BlockToCheck == BTYPE_RWATER2 || BlockToCheck == BTYPE_RWATER3 || BlockToCheck == BTYPE_AIR))
		return BTYPE_RWATER1;
	else if (bt == BTYPE_RWATER1 && (BlockToCheck == BTYPE_RWATER2 || BlockToCheck == BTYPE_RWATER3 || BlockToCheck == BTYPE_AIR))
		return BTYPE_RWATER2;
	else if (bt == BTYPE_RWATER2 && (BlockToCheck == BTYPE_RWATER3 || BlockToCheck == BTYPE_AIR))
		return BTYPE_RWATER3;
	return BTYPE_AIR;
}

void Chunk::WaterTick(int bloc)
{

	for (int x = 0; x < CHUNK_SIZE_X; ++x)
		for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			for (int y = 0; y < CHUNK_SIZE_Y; ++y)
			{
				BlockType bt = GetBlock(x, y, z);

				if (bt == BTYPE_WATER || bt == BTYPE_FWATER && TickCount == 0)
				{
					//AddWater(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					Water1(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
				}
				if (bt == BTYPE_RWATER1 && TickCount == 1)
				{
					//AddWater(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					Water2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
				}
				if (bt == BTYPE_RWATER2 && TickCount == 2)
				{
					//AddWater(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					Water2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
				}
				if (bt == BTYPE_RWATER3 && TickCount == 3)
				{
					//AddWater(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					Water2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
				}
			}
	TickCount++;
	if (TickCount > 3)
		TickCount = 0;
}

char Chunk::GetDirection(const Vector3<float> &Blockpos)
{
	int North = 0, South = 0, East = 0, West = 0;
	int i = 1;
	if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1) == BTYPE_WATER && GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z) == BTYPE_WATER &&
		GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1) == BTYPE_WATER && GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z) == BTYPE_WATER)
		return 'Q';
	BlockType BlockUnder;
	do
	{
		BlockUnder = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z + (i));
		if (BlockUnder == BTYPE_AIR || BlockUnder == BTYPE_RWATER1 || BlockUnder == BTYPE_RWATER2 || BlockUnder == BTYPE_RWATER3 || BlockUnder == BTYPE_WATER)
			North = i;

		if (i >= 4)
			North = i;

		+i++;
	} while (North == 0);
	i = 0;
	do
	{
		BlockUnder = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z - (i));
		if (BlockUnder == BTYPE_AIR || BlockUnder == BTYPE_RWATER1 || BlockUnder == BTYPE_RWATER2 || BlockUnder == BTYPE_RWATER3 || BlockUnder == BTYPE_WATER)
			South = i;
		if (i >= 4)
			South = i;
		i++;


	} while (South == 0);
	i = 0;
	do
	{
		BlockUnder = GetBlock(Blockpos.x - i, Blockpos.y - 1, Blockpos.z);
		if (BlockUnder == BTYPE_AIR || BlockUnder == BTYPE_RWATER1 || BlockUnder == BTYPE_RWATER2 || BlockUnder == BTYPE_RWATER3 || BlockUnder == BTYPE_WATER)
			West = i;

		if (i >= 4)
			West = i;
		i++;

	} while (West == 0);
	i = 0;
	do
	{
		BlockUnder = GetBlock(Blockpos.x + i, Blockpos.y - 1, Blockpos.z);
		if (BlockUnder == BTYPE_AIR || BlockUnder == BTYPE_RWATER1 || BlockUnder == BTYPE_RWATER2 || BlockUnder == BTYPE_RWATER3 || BlockUnder == BTYPE_WATER)
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


void Chunk::SetSource(int x, int y, int z, Vector3<float> source)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z)
	{
		m_blocks.SetSource(x, y, z, source);
	}
	else if (x <= -1 && m_negativeX)
	{
		m_negativeX->m_blocks.SetSource(CHUNK_SIZE_X + x, y, z, source);
	}
	else if (x >= CHUNK_SIZE_X && m_positiveX)
	{
		int distance = x - CHUNK_SIZE_X;
		m_positiveX->m_blocks.SetSource(distance, y, z, source);
	}
	else if (z <= -1 && m_negativeZ)
	{
		m_negativeZ->m_blocks.SetSource(x, y, CHUNK_SIZE_Z + z, source);

	}
	else if (z >= CHUNK_SIZE_Z && m_positiveZ)
	{
		int distance = z - CHUNK_SIZE_Z;
		m_positiveZ->m_blocks.SetSource(x, y, distance, source);
	}
}

Vector3<float> Chunk::GetSource(int x, int y, int z) const
{
	if (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z)
		return m_blocks.GetSource(x, y, z);

	else if (x >= -1 && m_negativeX)
		return m_negativeX->GetSource(CHUNK_SIZE_X + x, y, z);

	else if (x <= CHUNK_SIZE_X && m_positiveX)
		return m_positiveX->GetSource(x - CHUNK_SIZE_X - 1, y, z);

	else if (z <= -1 && m_negativeZ)
		return m_negativeZ->GetSource(x, y, CHUNK_SIZE_Z + z);

	else if (z >= CHUNK_SIZE_Z && m_positiveZ)
		return m_positiveZ->GetSource(x, y, z - CHUNK_SIZE_Z - 1);

	 return  m_blocks.GetSource(x, y, z);

}

void Chunk::RemoveWater(const Vector3<float> &Blockpos)
{
	for (int x = 0; x < CHUNK_SIZE_X; ++x)
		for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			for (int y = 0; y < CHUNK_SIZE_Y; ++y)
			{
				BlockType bt = GetBlock(x, y, z);
				if (bt >= 16 && bt <= 25)
				{
					bool BonChunk = false;
					Vector3<float> source = GetSource(x, y, z);
					Chunk* sourceChunk = this;
					int positionSourceX = source.x / 16;
					int positionSourceZ = source.z / 16;
					do
					{
						if (positionSourceX > m_position.x / 16)
							sourceChunk = sourceChunk->m_positiveX;
						else if (positionSourceX < m_position.x / 16)
							sourceChunk = sourceChunk->m_negativeX;
						else if (positionSourceZ > m_position.z / 16)
							sourceChunk = sourceChunk->m_positiveZ;
						else if (positionSourceZ < m_position.z / 16)
							sourceChunk = sourceChunk->m_negativeZ;
						else
							BonChunk;


					} while (BonChunk);
					
				}
			}
}






