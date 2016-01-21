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

void Model3d::LoadOBJ(std::string ModelPath, std::string TexPath)
{
	std::cout << "Loading " << ModelPath << "..." << std::endl;

	m_numFace = 0;
	m_numVerts = 0;
	m_numVertsTex = 0;
	m_numFaceTex = 0;

	//Alloue les tableaux 
	m_vertex = new V3<float>[30000];
	m_vertexTexture = new V3<float>[30000];
	m_face = new V3<int>[30000];
	m_faceTexture = new V3<int>[30000];

	// Open file
	std::ifstream file;
	file.open(ModelPath.c_str());

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
		else if (type == "vt")
		{
			ssline >> m_vertexTexture[m_numVertsTex].x >> m_vertexTexture[m_numVertsTex].y;
			m_numVertsTex++;
		}
		else if (type == "f")
		{
			int nbrSlash = std::count(line.begin(), line.end(), '/');

			if (nbrSlash == 0)
				ssline >> m_face[m_numFace].x >> m_face[m_numFace].y >> m_face[m_numFace].z;

			if (nbrSlash == 3)
			{
				std::string aaa = line;
				std::replace(aaa.begin(), aaa.end(), '/', ' ');
				ssline.str(aaa);
				ssline >> type;
				ssline >> m_face[m_numFace].x >> m_faceTexture[m_numFace].x >> m_face[m_numFace].y >> m_faceTexture[m_numFace].y >> m_face[m_numFace].z >> m_faceTexture[m_numFace].z;
			}
			m_numFace++;


		}
	}

	//Load la texture
	m_texture.Load(TexPath);
	if (!m_texture.IsValid())
		std::cerr << "Unable to load texture (" << TexPath << ")" << std::endl;

	m_loaded = true;
}

void Model3d::Render(float x, float y, float z, float rotH, float rotV, float r, float g, float b) const
{
	if (m_loaded)
	{
		m_texture.Bind();

		glPushMatrix();

		//Tanslation et rotation
		glTranslatef(x, y, z);
		glRotatef(rotH, 0, 1, 0);
		glRotatef(rotV, 1, 0, 0);
		glColor3f(r, g, b);

		glBegin(GL_TRIANGLES);

		for (int i = 0; i < m_numFace; i++)
		{
			glTexCoord2f(m_vertexTexture[m_faceTexture[i].x - 1].x, m_vertexTexture[m_faceTexture[i].x - 1].y);
			glVertex3f(m_vertex[m_face[i].x - 1].x, m_vertex[m_face[i].x - 1].y, m_vertex[m_face[i].x - 1].z);

			glTexCoord2f(m_vertexTexture[m_faceTexture[i].y - 1].x, m_vertexTexture[m_faceTexture[i].y - 1].y);
			glVertex3f(m_vertex[m_face[i].y - 1].x, m_vertex[m_face[i].y - 1].y, m_vertex[m_face[i].y - 1].z);

			glTexCoord2f(m_vertexTexture[m_faceTexture[i].z - 1].x, m_vertexTexture[m_faceTexture[i].z - 1].y);
			glVertex3f(m_vertex[m_face[i].z - 1].x, m_vertex[m_face[i].z - 1].y, m_vertex[m_face[i].z - 1].z);
		}

		glEnd();
		glPopMatrix();
	}
}
