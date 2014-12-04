#ifndef CHUNK_H__
#define CHUNK_H__
#include "array3d.h"
#include "chunkmesh.h"
#include "vector3.h"
#include "blockinfo.h"



class Chunk 
{
public:
    Chunk();
    ~Chunk();

    void RemoveBloc(int x, int y, int z);

	//La difference entre setblock et placeblock est que cette derniere vas placer un bloc dans de l'aire seulement
	//Un joueur vas placer un block 
	//Tandis que quand on load une map on set le block
    void SetBlock(int x, int y, int z, BlockType type);
	void PlaceBlock(int x, int y, int z, BlockType type);   

	void SetPosition(int x, int y, int z);
	Vector3<float> &GetPosition();
    BlockType GetBlock(int x, int y, int z);
	Vector3<float> &GetBlockPos(int x, int y, int z);
	void Update(BlockInfo* binfo);
	void AddBlockToMesh(ChunkMesh::VertexData * vd, int & count, BlockInfo binfo, int x, int y, int z);
	void Render() const;
	bool IsDirty() const;

private:
	Array3d<BlockType> m_blocks;
	bool m_isDirty;
	ChunkMesh m_chunkMesh;
	Vector3<float> m_position;

	


};

#endif // CHUNK_H__
