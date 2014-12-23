#ifndef ARRAY2D_H__
#define ARRAY2D_H__

#include "define.h"
#include <cassert>

template<class t>
class Array2d
{
public:
	Array2d(int x, int y);
	~Array2d();
	Array2d(const Array2d<t>& array);

	void Set(int x, int y, t value);
	t& Get(int x, int y);
	const t& Get(int x, int y) const;
	int GetIndex(t value) const;
	void Reset(t value);

private:
	int m_x, m_y;
	t* m_data;
};

template<class t>
Array2d<t>::Array2d(int x, int y) : m_x(x), m_y(y)
{
	m_data = new t[m_x * m_y ];	
}

template<class t>
Array2d<t>::~Array2d()
{
	delete[] m_data;
}

template<class t>
Array2d<t>::Array2d(const Array2d<t>& array)
{
	m_x = array.m_x;
	m_y = array.m_y;

	m_data = new t[m_x * m_y];
	for (int i = 0; i < m_x * m_y; ++i)
		m_data[i] = array.m_data[i];
}

template<class t>
void Array2d<t>::Set(int x, int y, t value)
{
	if(x >= 0 && y >= 0 && x < m_x && y < m_y);
		m_data[x + (y * m_x)] = value;
}

template<class t>
t& Array2d<t>::Get(int x, int y)
{
	assert(x >= 0 && y >= 0 && x < m_x && y < m_y);
		return m_data[x + (y * m_x)];
}

template<class t>
const t& Array2d<t>::Get(int x, int y) const
{
	assert(x >= 0 && y >= 0 && x < m_x && y < m_y);
	return m_data[x + (y * m_x)];
}
template<class t>
int Array2d<t>::GetIndex(t value) const
{
	for (int i = 0; i <m_x; i++)
	{
		if (Get(i, 0) == value)
			return i;
	}
	return -1;
}
template<class t>
void Array2d<t>::Reset(t value)
{
	for (int i = 0; i < m_x * m_y ; ++i)
		m_data[i] = value;
	
}

#endif // ARRAY2D_H__