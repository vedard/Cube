#ifndef CHUNK_H__
#define CHUNK_H__
#include "array3d.h"
#include "chunkmesh.h"
#include "vector3.h"
#include "blockinfo.h"
#include <stdlib.h>
#include <sstream>
#include <string>

class Chunk
{
public:
	Chunk();
	~Chunk();

	//Utiliser seuelemt par le player
	void RemoveBloc(int x, int y, int z);
	void PlaceBlock(int x, int y, int z, BlockType type);
	//---------------------------------------------------

	void SetBlock(int x, int y, int z, BlockType type, char direction);
	void SetPosition(int x, int y, int z);
	const Vector3<float> &GetPosition() const;
	const BlockType& GetBlock(int x, int y, int z) const;
	const char& GetDirection(int x, int y, int z) const;
	void Update(BlockInfo* &binfo);
	void AddBlockToMesh(ChunkMesh::VertexData * &vd, int& count,BlockInfo &binfo, const Vector3<float> &Blockpos);
	void RenderSolidBuffer(GLenum &m_program) const;
	void RenderTransparentBuffer(GLenum &m_program) const;
	void DeleteCache();
	bool NeedUpdate() const;
	bool& GetSave();
	void WaterTick(int bloc);


	Chunk* m_positiveX;
	Chunk* m_negativeX;
	Chunk* m_positiveZ;
	Chunk* m_negativeZ;
private:
	bool CheckFace(BlockType type, const Vector3<float> &Blockpos, const Vector3<float> &face) const;
	float CheckLightning(const Vector3<float> &Blockpos, const Vector3<float> &face) const;

	void SetExploded(int x, int y, int z, bool exploded);
	bool GetExploded(int x, int y, int z) const;

	void SetSource(int x, int y, int z, Vector3<float> source);
	Vector3<float> GetSource(int x, int y, int z) const;

	void Water1(const Vector3<float> &Blockpos);
	void Water2(const Vector3<float> &Blockpos);
	void WaterExploded(const Vector3<float> &Blockpos);
	BlockType WaterCheck(int x, int y, int z, BlockType bt);
	char GetDirection(const Vector3<float> &Blockpos);

	void RemoveWater(const Vector3<float> &Blockpos);



	//Todo - rendre private
public:
	bool m_iscreated;               //Si le chunk a été init selon le seed de la map

private:
	Array3d<BlockType> m_blocks;
	
	bool m_isDirty;					//Si il faut le reajouter au mesh
	bool m_save;					//Si faut le sauvegarder
	ChunkMesh m_chunkMesh;
	ChunkMesh m_transparentMesh;
	Vector3<float> m_position;
	BlockType m_defaultBlock; 				//On retourne ce block quand ya une erreur
	int TickCount;




};

#endif // CHUNK_H__
