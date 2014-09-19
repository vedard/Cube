#ifndef BLOCARRAY_H
#define BLOCARRAY_H

#include "blocinfo.h"
#include <iostream>
#include <string>
#include "define.h"

class BlocArray3d
{

public:
	BlocArray3d(int lenX, int lenY, int lenZ);
	BlocArray3d(const BlocArray3d &);
	~BlocArray3d();
	void Set(int x, int y, int z, BlocType type);
	BlocType Get(int x, int y, int z) const;
	void Reset(BlocType type);

private:

	//tableau 3d
	BlocType*** m_blocs = NULL;

	//Dimension du tableau
	int m_lenX;
	int m_lenY;
	int m_lenZ;

};

#endif
