#ifndef ARRAY3D_H__
#define ARRAY3D_H__

#include "define.h"

template<class t>
class Array3d
{
public:
	Array3d(int x, int y, int z);
	~Array3d();
	Array3d(const Array3d<t>& array);

	void Set(int x, int y, int z, t value);
	t Get(int x, int y, int z) const;

	void Reset(t value);

private:
	int m_x, m_y, m_z;
	t* m_data;

	t& GetElement(int x, int y, int z);
	const t& GetElement(int x, int y, int z) const;
};


template<class t>
Array3d<t>::Array3d(int x, int y, int z) : m_x(x), m_y(y), m_z(z)
{
	m_data = new t[m_x * m_y * m_z];

	Reset(0);
}

template<class t>
Array3d<t>::~Array3d()
{
	delete[] m_data;
}

template<class t>
Array3d<t>::Array3d(const Array3d& array)
{
	m_x = array.m_x;
	m_y = array.m_y;
	m_z = array.m_z;

	m_data = new t[m_x * m_y * m_z];
	for (int i = 0; i < m_x * m_y * m_z; ++i)
		m_data[i] = array.m_data[i];
}

template<class t>
void Array3d<t>::Set(int x, int y, int z, t value)
{
	if (x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z)
		GetElement(x, y, z) = value;
}

template<class t>
t Array3d<t>::Get(int x, int y, int z) const
{
	if (x >= 0 && y >= 0 && z >= 0 && x < m_x && y < m_y && z < m_z)
		return GetElement(x, y, z);
}

template<class t>
void Array3d<t>::Reset(t value)
{
	for (int i = 0; i < m_x * m_y * m_z; ++i)
		m_data[i] = value;
}

template<class t>
t& Array3d<t>::GetElement(int x, int y, int z)
{
	return m_data[x + (z * m_x) + (y * m_z * m_x)];
}

template<class t>
const t& Array3d<t>::GetElement(int x, int y, int z) const
{
	return m_data[x + (z * m_x) + (y * m_z * m_x)];
}

#endif // ARRAY3D_H__
