#include "chunk.h"

Chunk::Chunk() :m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z)
{

}

Chunk::~Chunk()
{
}

void Chunk::RemoveBloc(int x, int y, int z)
{
	m_blocks.Set(x, y, z, BTYPE_AIR);
}

void Chunk::SetBloc(int x, int y, int z, BlockType type)
{
	m_blocks.Set(x, y, z, type);
}

BlockType Chunk::GetBloc(int x, int y, int z)
{
	return m_blocks.Get(x, y, z);
}

