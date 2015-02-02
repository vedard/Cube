#ifndef BULLET_H__
#define BULLET_H__

#include "vector3.h"
#include "define.h"
#include "character.h"
#include "world.h"


class Bullet
{
public:
	Bullet();
	~Bullet();

	void Init(float x, float y, float z, float rotationVertical, float rotationHorizontal);

	void Update();
	bool CheckCollision(Character &character);
	bool CheckCollision(World &world);
	void Draw() const;

	const Vector3<float>& GetPosition() const;
	float GetDamage() const;
	bool GetIsActive() const;

	void SetInactive();


private:
	Vector3<float> m_pos;
	Vector3<float> m_vitesse;
	float m_HorizontalRot;
	float m_VerticalRot;

	bool m_isActive;

	float m_damage;
	float m_distance;
};

#endif //BULLET_H__