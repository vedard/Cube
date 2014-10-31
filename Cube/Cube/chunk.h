#ifndef CHUNK_H__
#define CHUNK_H__
#include "array3d.h"
#include "chunkmesh.h"



class Chunk 
{
public:
    Chunk();
    ~Chunk();

    void RemoveBloc(int x, int y, int z);
    void SetBlock(int x, int y, int z, BlockType type);
    BlockType GetBlock(int x, int y, int z);
	void Update();
	void AddBlockToMesh(ChunkMesh::VertexData * vd, int & count, BlockType bt, int x, int y, int z);
	void Render() const;
	bool IsDirty() const;

private:
	Array3d<BlockType> m_blocks;
	bool m_isDirty;
	ChunkMesh m_chunkMesh;
	


};

#endif // CHUNK_H__
