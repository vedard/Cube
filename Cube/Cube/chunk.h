#ifndef CHUNK_H__
#define CHUNK_H__
#include "array3d.h"
#include "chunkmesh.h"
#include "vector3.h"
#include "textureatlas.h"



class Chunk 
{
public:
    Chunk();
    ~Chunk();

    void RemoveBloc(int x, int y, int z);
    void SetBlock(int x, int y, int z, BlockType type);
	void SetPosition(int x, int y, int z);
    BlockType GetBlock(int x, int y, int z);
	void Update();
	void AddBlockToMesh(ChunkMesh::VertexData * vd, int & count, BlockType bt, int x, int y, int z);
	void Render() const;
	bool IsDirty() const;

private:
	Array3d<BlockType> m_blocks;
	bool m_isDirty;
	ChunkMesh m_chunkMesh;
	Vector3<float> m_position;
	


};

#endif // CHUNK_H__
