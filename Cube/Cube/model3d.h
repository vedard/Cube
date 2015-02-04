#ifndef MODEL3D_H__
#define MODEL3D_H__
#include "vector3.h"
#include "define.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Model3d
{
public:
	Model3d();
	~Model3d();
	void LoadOBJ(std::string filename);
	void Render(float x, float y, float z, float rotH, float rotV) const;

private:
	template <class t>
	struct V3
	{
		t x, y, z;

	};
	bool m_loaded;
	V3<float> *m_vertex;
	V3<float> *m_vertexTexture;
	V3<int> *m_face;
	V3<int> *m_faceTexture;


	int m_numVerts;
	int m_numFace;


};

#endif