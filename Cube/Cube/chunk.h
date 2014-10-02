#ifndef CHUNK_H__
#define CHUNK_H__
#include "blockarray3d.h"

class Chunk : public BlockArray3d
{
    public:
    Chunk();
    ~Chunk();

    void RemoveBloc(int x, int y, int z);
    void SetBloc(int x, int y, int z, BlockType type);
    BlockType GetBloc(int x, int y, int z);
};

#endif // CHUNK_H__
