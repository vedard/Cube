#ifndef MONSTER_H__
#define MONSTER_H__

#include "character.h"
#include "player.h"
#include "texture.h"
#include "model3d.h"

class Monster : public Character
{
public:
	Monster(MONSTER_TYPE type);
	~Monster();
	void Move(World &world);
	void Draw(Model3d &model, bool debugRange) const;
	void SetTarget( Character* target);
	void GetDamage(float damage);
	MONSTER_TYPE GetType();



private:
	Character* m_target;
	sf::Clock m_ClockAnimationDmg;
	MONSTER_TYPE type;
};
#endif // !MONSTER_H__


