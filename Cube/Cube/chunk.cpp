#include "chunk.h"
#include <iostream>

Chunk::Chunk() :m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_isDirty(true), m_chunkMesh(), m_position(0, 0, 0)
{
	m_blocks.Reset(BTYPE_AIR);


}

Chunk::~Chunk()
{

}

void Chunk::RemoveBloc(int x, int y, int z)
{
	m_blocks.Set(x, y, z, BTYPE_AIR);
	m_isDirty = true;
}

void Chunk::SetBlock(int x, int y, int z, BlockType type)
{
	m_blocks.Set(x, y, z, type);
	m_isDirty = true;
}

BlockType Chunk::GetBlock(int x, int y, int z)
{
	return m_blocks.Get(x, y, z);
}

void Chunk::SetPosition(int x, int y, int z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Chunk::Update(BlockInfo* binfo)
{
	// Update mesh
	if (m_isDirty)
	{
		int maxVertexCount = (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z) * (6 * 4);
		ChunkMesh::VertexData * vd = new ChunkMesh::VertexData[maxVertexCount];
		int count = 0;
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			{
				for (int y = 0; y < CHUNK_SIZE_Y; ++y)
				{
					if (count > USHRT_MAX)
						break;

					BlockType bt = GetBlock(x, y, z);

					if (bt != BTYPE_AIR)
					{
						if (x == 0 || y == 0 || z == 0 || x == CHUNK_SIZE_X - 1 || y == CHUNK_SIZE_Y - 1 || z == CHUNK_SIZE_Z - 1)
						{
							AddBlockToMesh(vd, count, binfo[bt], x + m_position.x, y + m_position.y, z + m_position.z);
						}
						else if (
							GetBlock(x + 1, y, z) == BTYPE_AIR || GetBlock(x + 1, y + 1, z) == BTYPE_AIR || GetBlock(x, y + 1, z) == BTYPE_AIR || GetBlock(x - 1, y + 1, z) == BTYPE_AIR || GetBlock(x - 1, y, z) == BTYPE_AIR || GetBlock(x - 1, y - 1, z) == BTYPE_AIR || GetBlock(x, y - 1, z) == BTYPE_AIR || GetBlock(x + 1, y - 1, z) == BTYPE_AIR ||
							GetBlock(x + 1, y, z + 1) == BTYPE_AIR || GetBlock(x + 1, y + 1, z + 1) == BTYPE_AIR || GetBlock(x, y + 1, z + 1) == BTYPE_AIR || GetBlock(x - 1, y + 1, z + 1) == BTYPE_AIR || GetBlock(x - 1, y, z + 1) == BTYPE_AIR || GetBlock(x - 1, y - 1, z + 1) == BTYPE_AIR || GetBlock(x, y - 1, z + 1) == BTYPE_AIR || GetBlock(x + 1, y - 1, z + 1) == BTYPE_AIR || GetBlock(x, y, z + 1) == BTYPE_AIR ||
							GetBlock(x + 1, y, z - 1) == BTYPE_AIR || GetBlock(x + 1, y + 1, z - 1) == BTYPE_AIR || GetBlock(x, y + 1, z - 1) == BTYPE_AIR || GetBlock(x - 1, y + 1, z - 1) == BTYPE_AIR || GetBlock(x - 1, y, z - 1) == BTYPE_AIR || GetBlock(x - 1, y - 1, z - 1) == BTYPE_AIR || GetBlock(x, y - 1, z - 1) == BTYPE_AIR || GetBlock(x + 1, y - 1, z - 1) == BTYPE_AIR || GetBlock(x, y, z - 1) == BTYPE_AIR
							)
						{
							AddBlockToMesh(vd, count, binfo[bt], x + m_position.x, y + m_position.y, z + m_position.z);
						}

					}
				}
			}
		}
		if (count > USHRT_MAX)
		{
			count = USHRT_MAX;
			std::cout << "[ Chunk :: Update ] Chunk data truncaned , too much vertices to have a 16 bit index " << std::endl;
		}
		m_chunkMesh.SetMeshData(vd, count);
		delete[] vd;
	}
	m_isDirty = false;
}

void Chunk::AddBlockToMesh(ChunkMesh::VertexData * vd, int & count, BlockInfo binfo, int x, int y, int z)
{
	

	// face
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 0.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f);
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 1.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .750f);
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 1.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .0f, binfo.v + binfo.h * .750f);
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 0.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .0f, binfo.v + binfo.h * .50f);

	// Droite
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 0.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .25f);
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 1.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .50f);
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 1.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f);
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 0.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .25f);

	//Gauche
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 0.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .25f);
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 0.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .25f);
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 1.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f);
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 1.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .50f);

	//Derirere
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 0.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .50f);
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 0.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .50f);
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 1.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .75f);
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 1.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f);

	//Haut
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 1.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f);
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 1.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * 1.0f);
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 1.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .00f, binfo.v + binfo.h * 1.0f);
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 1.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .00f, binfo.v + binfo.h * .75f);

	//Bas
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 0.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * .75f);
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 0.f, z + 0.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * .75f);
	vd[count++] = ChunkMesh::VertexData(x + 1.f, y + 0.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * 1.0f, binfo.v + binfo.h * 1.0f);
	vd[count++] = ChunkMesh::VertexData(x + 0.f, y + 0.f, z + 1.f, 1.f, 1.f, 1.f, binfo.u + binfo.w * .50f, binfo.v + binfo.h * 1.0f);

}

void Chunk::Render() const
{
	m_chunkMesh.Render();
}

bool Chunk::IsDirty() const
{
	return m_isDirty;
}
