#ifndef CREEPER_H__
#define CREEPER_H__
#include"monster.h"
class Creeper : public Monster
{
public:
	Creeper();
	~Creeper();
	void Explosion(World &world);
	void SetExplosionRadius(int radius);
	bool Attack(Character * character) override;
	void Move(World &world) override;


private:
	int explosionRadius = 6;
};
#endif // !

