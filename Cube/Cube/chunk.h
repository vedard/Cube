#ifndef CHUNK_H__
#define CHUNK_H__
#include "array3d.h"

class Chunk 
{
public:
    Chunk();
    ~Chunk();

    void RemoveBloc(int x, int y, int z);
    void SetBloc(int x, int y, int z, BlockType type);
    BlockType GetBloc(int x, int y, int z);

private:
	Array3d<BlockType> m_blocks;

};

#endif // CHUNK_H__
