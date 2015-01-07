#ifndef ENTITY_H__
#define ENTITY_H__

#include "define.h"
#include "vector3.h"
#include "world.h"

class Entity
{
public:
	Entity();
	~Entity();

	//Action
	void Spawn(World &world, int x, int z);
	void Move(World &world);
	bool CheckCollision(World &world) const;
	void Draw() const;

	//Get
	Vector3<float> GetPosition() const;
	Vector3<float> GetDimension() const;
	int GetHP() const;

private:
	
	Vector3<float> m_pos;
	Vector3<float> m_dimension;
	Vector3<float> m_vitesse;

	float m_HorizontalRot;
	float m_VerticalRot;

	int m_health;




};

#endif // MONSTER_H__

