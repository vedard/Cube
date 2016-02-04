#include "chunk.h"
#include <iostream>
#include <sstream>
#include <string>

char Chunk::QUIT = 'Q';

Chunk::Chunk() :m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_iscreated(false), m_isDirty(true), m_chunkMesh(), m_transparentMesh(), m_position(0, 0, 0), m_save(false), DeleteWater(false)
{
	m_blocks.Reset(BTYPE_AIR);
	m_defaultBlock = BTYPE_AIR;

}

Chunk::~Chunk()
{

}

void Chunk::RemoveBloc(int x, int y, int z)
{
	//FOR TESTING PURPOSES
	if (m_blocks.Get(x, y, z) == BTYPE_WATER)
	{
		Vector3<float> vf = Vector3<float>(x + m_position.x, y + m_position.y, z + m_position.z);
		RemoveWater(vf);
		DeleteWater = true;
		WaterSource = vf;
	}
	if (m_blocks.Get(x, y, z) == BTYPE_LAVA)
	{
		Vector3<float> vf = Vector3<float>(x + m_position.x, y + m_position.y, z + m_position.z);
		RemoveLava(vf);
		DeleteLava = true;
	}
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
		m_bInfo = binfo;
		m_isDirty = false;
		int maxVertexCount = (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z) * (6 * 4);

		//On creer un buffer pour les solid et un autre pour les transparents 
		ChunkMesh::VertexData * vds = new ChunkMesh::VertexData[maxVertexCount];
		vdt = new ChunkMesh::VertexData[maxVertexCount];
		int count_s = 0;
		count_t = 0;

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

		if (!Parametre::GetInstance().m_isServer)
		{
			m_chunkMesh.SetMeshData(vds, count_s);
			m_transparentMesh.SetMeshData(vdt, count_t);
		}
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


	if (faceType == BTYPE_AIR || faceType == BTYPE_LEAVE
		|| (
			(16 <= faceType && faceType <= 25)
			&& (type < 16 || type > 25)
			)
		|| (
			(16 > faceType || faceType > 25)
			&& (type >= 16 && type <= 25)
			)
		||
		(faceType == BTYPE_RLAVA1 && type == BTYPE_LAVA) ||
		(faceType == BTYPE_RLAVA3 && type == BTYPE_RLAVA2)
		|| (faceType == BTYPE_RWATER1 && type == BTYPE_WATER) ||
		(faceType == BTYPE_RLAVA2 && type == BTYPE_RLAVA1)
		||
		(faceType == BTYPE_RWATER3 && type == BTYPE_RWATER2)
		||
		(faceType == BTYPE_RWATER2 && type == BTYPE_RWATER1)
		||
		(faceType == BTYPE_RWATER1 && type == BTYPE_FWATER)
		||
		(faceType == BTYPE_RLAVA1 && type == BTYPE_FLAVA)
		)
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
		return Chunk::QUIT;
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
					BlockType checkBlock = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z);
					if (checkBlock == BTYPE_AIR || (BTYPE_RWATER1 < checkBlock && checkBlock <= BTYPE_RWATER3))
					{
						SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z, BTYPE_RWATER1, direction);
						SetExploded(Blockpos.x + 1, Blockpos.y, Blockpos.z, true);
					}
					checkBlock = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z);
					if (checkBlock == BTYPE_AIR || (BTYPE_RWATER1 < checkBlock && checkBlock <= BTYPE_RWATER3))
					{
						SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z, BTYPE_RWATER1, direction);
						SetExploded(Blockpos.x - 1, Blockpos.y, Blockpos.z, true);
					}
					checkBlock = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1);
					if (checkBlock == BTYPE_AIR || (BTYPE_RWATER1 < checkBlock && checkBlock <= BTYPE_RWATER3))
					{
						SetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1, BTYPE_RWATER1, direction);
						SetExploded(Blockpos.x, Blockpos.y, Blockpos.z + 1, true);

					}
					checkBlock = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1);
					if (checkBlock == BTYPE_AIR || (BTYPE_RWATER1 < checkBlock && checkBlock <= BTYPE_RWATER3))
					{
						SetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1, BTYPE_RWATER1, direction);
						SetExploded(Blockpos.x, Blockpos.y, Blockpos.z - 1, true);
					}
				}
				else
				{
					if (GetBlock(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif) == BTYPE_AIR)
					{
						SetBlock(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif, BTYPE_RWATER1, direction);
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
	}
	else if (blockUnder != BTYPE_FWATER && blockUnder != BTYPE_WATER)
	{
		BlockType bt = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z);
		BlockType blockToAdd = WaterCheck(Blockpos.x, Blockpos.y, Blockpos.z + 1, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			BlockType checkBlock = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1);
			if (checkBlock == BTYPE_AIR)//|| (blockToAdd > checkBlock && checkBlock <= BTYPE_RWATER3) )
			{
				SetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1, blockToAdd, 'N');
				SetExploded(Blockpos.x, Blockpos.y, Blockpos.z + 1, true);
			}
		}
		blockToAdd = WaterCheck(Blockpos.x, Blockpos.y, Blockpos.z - 1, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			BlockType checkBlock = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1);
			if (checkBlock == BTYPE_AIR)// || (blockToAdd > checkBlock && checkBlock <= BTYPE_RWATER3))
			{
				SetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1, blockToAdd, 'S');
				SetExploded(Blockpos.x, Blockpos.y, Blockpos.z - 1, true);
			}
		}
		blockToAdd = WaterCheck(Blockpos.x + 1, Blockpos.y, Blockpos.z, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			BlockType checkBlock = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z);
			if (checkBlock == BTYPE_AIR)// || (blockToAdd > checkBlock && checkBlock <= BTYPE_RWATER3))
			{
				SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z, blockToAdd, 'E');
				SetExploded(Blockpos.x + 1, Blockpos.y, Blockpos.z, true);
			}
		}
		blockToAdd = WaterCheck(Blockpos.x - 1, Blockpos.y, Blockpos.z, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			BlockType checkBlock = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z);
			if (checkBlock == BTYPE_AIR)// || (blockToAdd > checkBlock && checkBlock <= BTYPE_RWATER3))
			{
				SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z, blockToAdd, 'W');
				SetExploded(Blockpos.x - 1, Blockpos.y, Blockpos.z, true);
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
				try
				{
					BlockType bt = GetBlock(x, y, z);


					if ((bt == BTYPE_WATER || bt == BTYPE_FWATER) && bloc == 0)
					{
						Water1(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
					else if (bt == BTYPE_RWATER1 && bloc == 1)
					{
						Water2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
					else if (bt == BTYPE_RWATER2 && bloc == 2)
					{
						Water2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
					else if (bt == BTYPE_RWATER3 && bloc == 3)
					{
						Water2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
					else if ((bt == BTYPE_LAVA || bt == BTYPE_FLAVA) && bloc == 0)
					{
						Lava1(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
					else if (bt == BTYPE_RLAVA1 && bloc == 1)
					{
						Lava2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
					else if (bt == BTYPE_RLAVA2 && bloc == 2)
					{
						Lava2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
					else if (bt == BTYPE_RLAVA3 && bloc == 3)
					{
						Lava2(Vector3<float>((int)x % CHUNK_SIZE_X, (int)y % CHUNK_SIZE_Y, (int)z % CHUNK_SIZE_Z));
					}
				}
				catch (...)
				{

				}
			}

}

char Chunk::GetDirection(const Vector3<float> &Blockpos)
{
	int North = 0, South = 0, East = 0, West = 0;
	int i = 1;
	if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1) == BTYPE_WATER && GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z) == BTYPE_WATER &&
		GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1) == BTYPE_WATER && GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z) == BTYPE_WATER)
		return 'Q';
	if (GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1) == BTYPE_LAVA && GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z) == BTYPE_LAVA &&
		GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1) == BTYPE_LAVA && GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z) == BTYPE_LAVA)
		return 'Q';
	BlockType BlockUnder;
	do
	{
		BlockUnder = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z + (i));
		if (BlockUnder == BTYPE_AIR || BlockUnder == BTYPE_RWATER1 || BlockUnder == BTYPE_RWATER2 || BlockUnder == BTYPE_RWATER3 || BlockUnder == BTYPE_WATER || BlockUnder == BTYPE_RLAVA1 || BlockUnder == BTYPE_RLAVA2 || BlockUnder == BTYPE_RLAVA3 || BlockUnder == BTYPE_LAVA)
			North = i;

		if (i >= 4)
			North = i;

		+i++;
	} while (North == 0);
	i = 0;
	do
	{
		BlockUnder = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z - (i));
		if (BlockUnder == BTYPE_AIR || BlockUnder == BTYPE_RWATER1 || BlockUnder == BTYPE_RWATER2 || BlockUnder == BTYPE_RWATER3 || BlockUnder == BTYPE_WATER || BlockUnder == BTYPE_RLAVA1 || BlockUnder == BTYPE_RLAVA2 || BlockUnder == BTYPE_RLAVA3 || BlockUnder == BTYPE_LAVA)
			South = i;
		if (i >= 4)
			South = i;
		i++;


	} while (South == 0);
	i = 0;
	do
	{
		BlockUnder = GetBlock(Blockpos.x - i, Blockpos.y - 1, Blockpos.z);
		if (BlockUnder == BTYPE_AIR || BlockUnder == BTYPE_RWATER1 || BlockUnder == BTYPE_RWATER2 || BlockUnder == BTYPE_RWATER3 || BlockUnder == BTYPE_WATER || BlockUnder == BTYPE_RLAVA1 || BlockUnder == BTYPE_RLAVA2 || BlockUnder == BTYPE_RLAVA3 || BlockUnder == BTYPE_LAVA)
			West = i;

		if (i >= 4)
			West = i;
		i++;

	} while (West == 0);
	i = 0;
	do
	{
		BlockUnder = GetBlock(Blockpos.x + i, Blockpos.y - 1, Blockpos.z);
		if (BlockUnder == BTYPE_AIR || BlockUnder == BTYPE_RWATER1 || BlockUnder == BTYPE_RWATER2 || BlockUnder == BTYPE_RWATER3 || BlockUnder == BTYPE_WATER || BlockUnder == BTYPE_RLAVA1 || BlockUnder == BTYPE_RLAVA2 || BlockUnder == BTYPE_RLAVA3 || BlockUnder == BTYPE_LAVA)
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

void Chunk::RemoveWater(Vector3<float> vf)
{
	Chunk* chunk = this;
	for (int yp = vf.y; yp > 0; --yp)
		for (int zp = 0; zp < CHUNK_SIZE_Z; ++zp)
			for (int xp = 0; xp < CHUNK_SIZE_X; ++xp)
			{
				if (GetBlock(xp, yp, zp) >= 17 && GetBlock(xp, yp, zp) <= 20)
				{
					if (chunk->GetBlock(xp, yp, zp) > BTYPE_WATER && chunk->GetBlock(xp, yp, zp) <= BTYPE_FWATER)
						chunk->SetBlock(xp, yp, zp, BTYPE_AIR, 'Q');
					else if (chunk->GetBlock(xp, yp, zp) == BTYPE_WATER)
						chunk->SetExploded(xp, yp, zp, false);
				}
			}
}

void Chunk::RemoveLava(Vector3<float> vf)
{
	Chunk* chunk = this;
	for (int yp = vf.y; yp > 0; --yp)
		for (int zp = 0; zp < CHUNK_SIZE_Z; ++zp)
			for (int xp = 0; xp < CHUNK_SIZE_X; ++xp)
			{
				if (GetBlock(xp, yp, zp) >= 17 && GetBlock(xp, yp, zp) <= 20)
				{
					if (chunk->GetBlock(xp, yp, zp) < BTYPE_LAVA && chunk->GetBlock(xp, yp, zp) >= BTYPE_RLAVA1)
						chunk->SetBlock(xp, yp, zp, BTYPE_AIR, 'Q');
					else if (chunk->GetBlock(xp, yp, zp) == BTYPE_LAVA)
						chunk->SetExploded(xp, yp, zp, false);
				}
			}
}

void Chunk::Lava1(const Vector3<float> &Blockpos)
{
	BlockType bt = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z);
	if (bt != BTYPE_FLAVA)
	{
		char direction = GetDirection(Blockpos.x, Blockpos.y, Blockpos.z);
		if (bt == BTYPE_AIR)
		{
			SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z, BTYPE_FLAVA, direction);
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

				bool bExplode = true;
				for (int i = 1; i < 4; i++)
				{
					BlockType blockkk = GetBlock(Blockpos.x + (i * xModif), Blockpos.y - 1, Blockpos.z + (i * zModif));
					if (blockkk == BTYPE_AIR || blockkk == BTYPE_RLAVA1 || blockkk == BTYPE_RLAVA2 || blockkk == BTYPE_RLAVA3 || blockkk == BTYPE_LAVA || blockkk == BTYPE_FLAVA)
					{
						bExplode = false;
						break;
					}
				}
				if (bExplode == true)
				{
					SetExploded(Blockpos.x, Blockpos.y, Blockpos.z, true);
					BlockType checkBlock = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z);
					if (checkBlock == BTYPE_AIR || (BTYPE_RLAVA1 < checkBlock && checkBlock <= BTYPE_RLAVA3))
					{
						SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z, BTYPE_RLAVA1, direction);
						SetExploded(Blockpos.x + 1, Blockpos.y, Blockpos.z, true);
					}
					checkBlock = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z);
					if (checkBlock == BTYPE_AIR || (BTYPE_RLAVA1 < checkBlock && checkBlock <= BTYPE_RLAVA3))
					{
						SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z, BTYPE_RLAVA1, direction);
						SetExploded(Blockpos.x - 1, Blockpos.y, Blockpos.z, true);
					}
					checkBlock = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1);
					if (checkBlock == BTYPE_AIR || (BTYPE_RLAVA1 < checkBlock && checkBlock <= BTYPE_RLAVA3))
					{
						SetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1, BTYPE_RLAVA1, direction);
						SetExploded(Blockpos.x, Blockpos.y, Blockpos.z + 1, true);

					}
					checkBlock = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1);
					if (checkBlock == BTYPE_AIR || (BTYPE_RLAVA1 < checkBlock && checkBlock <= BTYPE_RLAVA3))
					{
						SetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1, BTYPE_RLAVA1, direction);
						SetExploded(Blockpos.x, Blockpos.y, Blockpos.z - 1, true);
					}
				}
				else
				{
					if (GetBlock(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif) == BTYPE_AIR)
					{
						SetBlock(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif, BTYPE_RLAVA1, direction);
					}
				}
			}
		}
	}
}

void Chunk::Lava2(const Vector3<float> &Blockpos)
{
	if (GetExploded(Blockpos.x, Blockpos.y, Blockpos.z) == true)
		LavaExploded(Blockpos);
	else
	{
		char direction = GetDirection(Blockpos.x, Blockpos.y, Blockpos.z);
		BlockType bt = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z);
		if (bt == BTYPE_AIR || bt == BTYPE_RLAVA1 || bt == BTYPE_RLAVA2 || bt == BTYPE_RLAVA3)
		{
			SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z, BTYPE_FLAVA, direction);
		}
		else if (bt != BTYPE_FLAVA && bt != BTYPE_LAVA)
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
			BlockType blockToAdd = LavaCheck(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif, GetBlock(Blockpos.x, Blockpos.y, Blockpos.z));
			if (blockToAdd != BTYPE_AIR)
			{
				if (GetBlock(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif) == BTYPE_AIR)
				{
					SetBlock(Blockpos.x + xModif, Blockpos.y, Blockpos.z + zModif, blockToAdd, direction);

				}
			}
		}
	}
}

void Chunk::LavaExploded(const Vector3<float> &Blockpos)
{
	BlockType blockUnder = GetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z);
	if (blockUnder == BTYPE_AIR || blockUnder == BTYPE_RLAVA1 || blockUnder == BTYPE_RLAVA2 || blockUnder == BTYPE_RLAVA3)
	{
		SetBlock(Blockpos.x, Blockpos.y - 1, Blockpos.z, BTYPE_FLAVA, GetDirection(Blockpos.x, Blockpos.y, Blockpos.z));
		SetExploded(Blockpos.x, Blockpos.y - 1, Blockpos.z, false);
	}
	else if (blockUnder != BTYPE_FLAVA && blockUnder != BTYPE_LAVA)
	{
		BlockType bt = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z);
		BlockType blockToAdd = LavaCheck(Blockpos.x, Blockpos.y, Blockpos.z + 1, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			BlockType checkBlock = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1);
			if (checkBlock == BTYPE_AIR)//|| (blockToAdd > checkBlock && checkBlock <= BTYPE_RWATER3) )
			{
				SetBlock(Blockpos.x, Blockpos.y, Blockpos.z + 1, blockToAdd, 'N');
				SetExploded(Blockpos.x, Blockpos.y, Blockpos.z + 1, true);
			}
		}
		blockToAdd = LavaCheck(Blockpos.x, Blockpos.y, Blockpos.z - 1, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			BlockType checkBlock = GetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1);
			if (checkBlock == BTYPE_AIR)// || (blockToAdd > checkBlock && checkBlock <= BTYPE_RWATER3))
			{
				SetBlock(Blockpos.x, Blockpos.y, Blockpos.z - 1, blockToAdd, 'S');
				SetExploded(Blockpos.x, Blockpos.y, Blockpos.z - 1, true);
			}
		}
		blockToAdd = LavaCheck(Blockpos.x + 1, Blockpos.y, Blockpos.z, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			BlockType checkBlock = GetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z);
			if (checkBlock == BTYPE_AIR)// || (blockToAdd > checkBlock && checkBlock <= BTYPE_RWATER3))
			{
				SetBlock(Blockpos.x + 1, Blockpos.y, Blockpos.z, blockToAdd, 'E');
				SetExploded(Blockpos.x + 1, Blockpos.y, Blockpos.z, true);
			}
		}
		blockToAdd = LavaCheck(Blockpos.x - 1, Blockpos.y, Blockpos.z, bt);
		if (blockToAdd != BTYPE_AIR)
		{
			BlockType checkBlock = GetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z);
			if (checkBlock == BTYPE_AIR)// || (blockToAdd > checkBlock && checkBlock <= BTYPE_RWATER3))
			{
				SetBlock(Blockpos.x - 1, Blockpos.y, Blockpos.z, blockToAdd, 'W');
				SetExploded(Blockpos.x - 1, Blockpos.y, Blockpos.z, true);
			}
		}
	}
}

BlockType Chunk::LavaCheck(int x, int y, int z, BlockType bt)
{
	BlockType BlockToCheck = GetBlock(x, y, z);
	if (bt == BTYPE_FLAVA && (BlockToCheck == BTYPE_RLAVA1 || BlockToCheck == BTYPE_RLAVA2 || BlockToCheck == BTYPE_RLAVA3 || BlockToCheck == BTYPE_AIR))
		return BTYPE_RLAVA1;
	else if (bt == BTYPE_LAVA && (BlockToCheck == BTYPE_RLAVA1 || BlockToCheck == BTYPE_RLAVA2 || BlockToCheck == BTYPE_RLAVA3 || BlockToCheck == BTYPE_AIR))
		return BTYPE_RLAVA1;
	else if (bt == BTYPE_RLAVA1 && (BlockToCheck == BTYPE_RLAVA2 || BlockToCheck == BTYPE_RLAVA3 || BlockToCheck == BTYPE_AIR))
		return BTYPE_RLAVA2;
	else if (bt == BTYPE_RLAVA2 && (BlockToCheck == BTYPE_RLAVA3 || BlockToCheck == BTYPE_AIR))
		return BTYPE_RLAVA3;
	return BTYPE_AIR;
}








