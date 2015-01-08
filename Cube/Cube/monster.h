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
	void Move(World &world, Player &player);
	void Draw() const;

private:
	Texture m_texture;
	Vector3<float> m_target;

};
#endif // !MONSTER_H__


