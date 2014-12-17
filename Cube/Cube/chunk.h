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

	//Utiliser seuelemt par le player
	void RemoveBloc(int x, int y, int z);
	void PlaceBlock(int x, int y, int z, BlockType type);
	//---------------------------------------------------

	void SetBlock(int x, int y, int z, BlockType type);
	void SetPosition(int x, int y, int z);
	Vector3<float> &GetPosition();
	BlockType GetBlock(int x, int y, int z);
	Vector3<float> &GetBlockPos(int x, int y, int z);
	void Update(BlockInfo* &binfo);
	void AddBlockToMesh(ChunkMesh::VertexData * &vd, int & count, BlockInfo &binfo, int x, int y, int z);
	void Render(GLenum &m_program) const;
	bool IsDirty() const;
	bool& GetSave();


	Chunk* m_positiveX;
	Chunk* m_negativeX;
	Chunk* m_positiveZ;
	Chunk* m_negativeZ;

private:
	Array3d<BlockType> m_blocks;
	bool m_isDirty;					//Si il faut le reajouter au mesh
	bool m_save;					//Si faut le sauvegarder
	ChunkMesh m_chunkMesh;
	Vector3<float> m_position;




};

#endif // CHUNK_H__
