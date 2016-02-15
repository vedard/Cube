#ifndef BEAR_H__
#define BEAR_H__

#include "character.h"
#include "model3d.h"
#include "sound.h"
#include "animal.h"

class Bear : public Animal
{
public:
	Bear();
	~Bear();
	void Move(World &world) override;
	void GetDamage(float damage) override;
	void SetTarget(Character* target) override;
	private:
		bool isHurt;
		int chillCount;
		Character* m_target;

};

#endif