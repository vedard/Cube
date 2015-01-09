#ifndef MONSTER_H__
#define MONSTER_H__

#include "character.h"
#include "player.h"
#include "texture.h"

class Monster : public Character
{
public:
	Monster();
	~Monster();
	void Move(World &world);
	void Draw(bool debugRange) const;
	void SetTarget(Character* target);
	


private:
	Texture m_texture;
	Character* m_target;

};
#endif // !MONSTER_H__

