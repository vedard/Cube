#ifndef CHICKEN_H__
#define CHICKEN_H__

#include "animal.h"

class Chicken : public Animal
{
public:
	Chicken();
	~Chicken();
	void Move(World &world) override;
	void SetTarget(Character* target) override;
	bool GetDamage(float damage, bool ignoreArmor, bool godMode, Character* killer, Sound::ListeSons son, bool playonce) override;

private:
	Character* m_target;
	int chillCount;
	bool isHurt;

};
#endif // !COW_H__
#pragma once

