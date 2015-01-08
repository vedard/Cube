#ifndef CHARACTER_H__
#define CHARACTER_H__

#include "define.h"
#include "vector3.h"
#include "world.h"

class Character
{
public:
	Character();
	~Character();

	//Action
	void Spawn(World &world, int x, int z);
	void Move(World &world);
	bool CheckCollision(World &world) const;
	bool CheckCollision(Character & character) const;
	void Draw() const;
	void Attack(Character * character, int damage) const;
	void GetDamage(int damage);

	//Get
	const Vector3<float>& GetPosition() const;
	const Vector3<float>& GetDimension() const;
	int GetHP() const;
	float GetHorizontalRotation() const;
	float GetVerticalRotation() const;

protected:
	
	Vector3<float> m_pos;
	Vector3<float> m_dimension;
	Vector3<float> m_vitesse;

	float m_HorizontalRot;
	float m_VerticalRot;

	int m_health;




};

#endif // MONSTER_H__

