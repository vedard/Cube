#ifndef DRAGON_H__
#define DRAGON_H__

#include "character.h"
#include "model3d.h"
#include "sound.h"
#include "animal.h"

class Dragon : public Animal
{
public:
	Dragon();
	~Dragon();
	void Move(World &world) override;
	bool GetDamage(float damage, bool ignoreArmor, bool godMode, Sound::ListeSons son, bool playonce)override;
	void SetTarget(Character* target) override;
private:
	bool isHurt;
	int chillCount;
	Character* m_target;
	bool m_falling;
	bool m_up;
	int hauteury;

};


#endif // !DRAGON_H__
