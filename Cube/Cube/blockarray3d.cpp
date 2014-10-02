#include "blockarray3d.h"

BlockArray3d::BlockArray3d(int x, int y, int z) : m_x(x), m_y(y), m_z(z)
{
    m_blocs = new BlockType[m_x * m_y * m_z];

    Reset(BTYPE_AIR);
}

BlockArray3d::~BlockArray3d()
{
    delete [] m_blocs;
}

BlockArray3d::BlockArray3d(const BlockArray3d& array)
{
    m_x = array.m_x;
    m_y = array.m_y;
    m_z = array.m_z;

    m_blocs = new BlockType[m_x * m_y * m_z];
    for(int i = 0; i < m_x * m_y * m_z; ++i)
        m_blocs[i] = array.m_blocs[i];
}

void BlockArray3d::Set(int x, int y, int z, BlockType type)
{
    GetElement(x, y, z) = type;
}

BlockType BlockArray3d::Get(int x, int y, int z) const
{
    return GetElement(x, y, z);
}

void BlockArray3d::Reset(BlockType type)
{
    for(int i = 0; i < m_x * m_y * m_z; ++i)
        m_blocs[i] = type;
}

BlockType& BlockArray3d::GetElement(int x, int y, int z)
{
    return m_blocs[x + (z * m_x) + (y * m_z * m_x)];
}

const BlockType& BlockArray3d::GetElement(int x, int y, int z) const
{
    return m_blocs[x + (z * m_x) + (y * m_z * m_x)];
}

