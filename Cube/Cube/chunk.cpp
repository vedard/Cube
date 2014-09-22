#include "chunk.h"


Chunk::Chunk() : BlocArray3d(CHUNK_SIZE_X,CHUNK_SIZE_Y,CHUNK_SIZE_Z)
{

}

Chunk::~Chunk()
{

}

void Chunk::RemoveBloc(int x, int y, int z)
{
	Set(x, y, z, BlocType::BTYPE_AIR);
}

void Chunk::SetBloc(int x, int y, int z, BlocType type)
{
	Set(x, y, z, type);
}

BlocType Chunk::GetBloc(int x, int y, int z)
{
	return Get(x, y, z);
}