#include "blocArray3d.h"

BlocArray3d::BlocArray3d(int lenX, int lenY, int lenZ) : m_lenX(lenX), m_lenY(lenY), m_lenZ(lenZ)
{
	//Alloue la memoire pour un tableau 3d
	m_blocs = new BlocType**[lenX];
	for (int i = 0; i < lenX; i++)
	{
		m_blocs[i] = new BlocType*[lenY];
		for (int j = 0; j < lenY; j++)
			m_blocs[i][j] = new BlocType[lenZ];

	}

	//Initialise le tableau 3d
	for (int i = 0; i < lenX; i++)
		for (int j = 0; j < lenY; j++)
			for (int k = 0; k < lenZ; k++)
				m_blocs[i][j][k] = BlocType::BTYPE_AIR;
}
BlocArray3d::BlocArray3d(const BlocArray3d & blocArray3dSource)
{
	//Alloue un nouveau tableau
	m_blocs = new BlocType**[blocArray3dSource.m_lenX];
	for (int i = 0; i < blocArray3dSource.m_lenX; i++)
	{
		m_blocs[i] = new BlocType*[blocArray3dSource.m_lenY];
		for (int j = 0; j < blocArray3dSource.m_lenY; j++)
			m_blocs[i][j] = new BlocType[blocArray3dSource.m_lenZ];

	}

	//Copie les tableaux
	for (int i = 0; i < blocArray3dSource.m_lenX; i++)
		for (int j = 0; j < blocArray3dSource.m_lenY; j++)
			for (int k = 0; k < blocArray3dSource.m_lenZ; k++)
				m_blocs[i][j][k] = blocArray3dSource.Get(i, j, k);
	
	//Copie les variables
	m_lenX = blocArray3dSource.m_lenX;
	m_lenY = blocArray3dSource.m_lenY;
	m_lenZ = blocArray3dSource.m_lenZ;

}
BlocArray3d::~BlocArray3d()
{
	//Desalloue la memoire
	for (int i = 0; i < m_lenX; i++)
	{
		for (int j = 0; j < m_lenY; j++)
			delete[] m_blocs[i][j];

		delete[] m_blocs[i];
	}
	delete[] m_blocs;
}

void BlocArray3d::Set(int x, int y, int z, BlocType type)
{
	m_blocs[x][y][z] = type;
}

BlocType BlocArray3d::Get(int x, int y, int z) const
{

	return m_blocs[x][y][z];
}

void BlocArray3d::Reset(BlocType type)
{
	for (int i = 0; i < m_lenX; i++)
		for (int j = 0; j < m_lenY; j++)
			for (int k = 0; k < m_lenZ; k++)
				m_blocs[i][j][k] = type;
}