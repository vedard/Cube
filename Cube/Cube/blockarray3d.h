#ifndef BLOCKARRAY3D_H__
#define BLOCKARRAY3D_H__

#include "define.h"

class BlockArray3d
{
    public:
    BlockArray3d(int x, int y, int z);
    ~BlockArray3d();
    BlockArray3d(const BlockArray3d& array);

    void Set(int x, int y, int z, BlockType type);
    BlockType Get(int x, int y, int z) const;

    void Reset(BlockType type);

    private:
        int m_x, m_y, m_z;
        BlockType* m_blocs;

        BlockType& GetElement(int x, int y, int z);
        const BlockType& GetElement(int x, int y, int z) const;
};

#endif // BLOCKARRAY3D_H__
