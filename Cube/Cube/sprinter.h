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
	int m_mouvementCooldown;
};
#endif //SPRINTER_H__

