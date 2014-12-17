#ifndef CHUNKMESH_H__
#define CHUNKMESH_H__
#include "define.h"

class ChunkMesh
{
public:
	// Structure représentant toutes les informations de chacuns des vertex
	// S'assurer que le size de cette struct reste un multiple de 32
	// octet pour un maximum de performance
	// Au besoin, ajouter du padding
	struct VertexData
	{
		float x, y, z;
		float r, g, b;
		float u, v;
		float type;

		VertexData() {}
		VertexData(float _x, float _y, float _z, float _r, float _g, float _b, float _u, float _v, float _type) : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), u(_u), v(_v), type(_type) {}
	};

public:
	ChunkMesh();
	~ChunkMesh();

	bool IsValid() const;
	void SetMeshData(VertexData* vd, int vertexCount);
	void Render(GLenum &m_program) const;
	void DeleteBuffer();

	int Count() const;

private:
	bool m_isValid;
	int m_vertexCount;
	GLuint m_vertexVboId;
	GLuint m_indexVboId;
};

#endif // CHUNKMESH_H__
