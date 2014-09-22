#ifndef CHUNK_H
#define CHUNK_H
#include "blocArray3d.h"

class Chunk : public BlocArray3d
{
public:
	Chunk();
	~Chunk();
	void RemoveBloc(int x, int y, int z);
	void SetBloc(int x, int y, int z, BlocType type);
	BlocType GetBloc(int x, int y, int z);
private:

};

#endif