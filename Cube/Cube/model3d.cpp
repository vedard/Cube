#include "model3d.h"


Model3d::Model3d() :
m_numFace(0),
m_numVerts(0)
{
	m_vertex = NULL;
	m_face = NULL;
}


Model3d::~Model3d()
{
	delete[] m_vertex;
	delete[] m_face;
}

void Model3d::LoadOBJ(std::string filename)
{
	std::cout << "Loading " << filename << "..." << std::endl;
	m_vertex = new V3<float>[10000];
	m_face = new V3<int>[10000];

	// Open file
	std::ifstream file;
	file.open(filename.c_str());

	//Read All file
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::stringstream ss(str);

	//Close it
	file.close();

	//Read
	while (ss)
	{
		std::string line;
		std::getline(ss, line, '\n');
		std::stringstream ssline(line);

		std::string type = "";
		ssline >> type;
		if (type == "v")
		{
			ssline >> m_vertex[m_numVerts].x >> m_vertex[m_numVerts].y >> m_vertex[m_numVerts].z;
			m_numVerts++;
		}
		else if (type == "f")
		{
			ssline >> m_face[m_numFace].x >> m_face[m_numFace].y >> m_face[m_numFace].z;
			m_numFace++;
		}

	}



	m_loaded = true;
}

void Model3d::Render(float x, float y, float z, float rotH, float rotV) const
{
	if (m_loaded)
	{
		glPushMatrix();

		glTranslatef(x, y, z);
		glRotatef(rotH, 0.f, 1.f, 0.f);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < m_numFace; i++)
		{
			glVertex3f(m_vertex[m_face[i].x - 1].x, m_vertex[m_face[i].x - 1].y, m_vertex[m_face[i].x - 1].z);
			glVertex3f(m_vertex[m_face[i].y - 1].x, m_vertex[m_face[i].y - 1].y, m_vertex[m_face[i].y - 1].z);
			glVertex3f(m_vertex[m_face[i].z - 1].x, m_vertex[m_face[i].z - 1].y, m_vertex[m_face[i].z - 1].z);
		}
		glEnd();
		glPopMatrix();
	}
}