#ifndef SPRINTER_H__
#define SPRINTER_H__
#include"monster.h"
class Sprinter : public Monster
{
public:
	Sprinter();
	~Sprinter();
	void Move(World &world);

private:
};
#endif //SPRINTER_H__

