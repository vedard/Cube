#ifndef CHARACTER_H__
#define CHARACTER_H__

#include "define.h"
#include "vector3.h"
#include "world.h"
#include "sound.h"

class Character
{
public:
	Character();
	~Character();

	//Action
	void Spawn(World &world, int x, int z);
	virtual void Move(World &world);
	bool CheckCollision(World &world) const;
	bool CheckCollision(Character & character) const;
	void Draw() const;
	bool Attack(Character * character, float damage);
	bool Attack(Character * character);
	virtual bool GetDamage(float damage, bool ignoreArmor, bool godMode , Sound::ListeSons son = Sound::ListeSons::HURT, bool playonce = false);
	void Jump();

	//Set
	void SetName(std::string name);

	//Get   
	const Vector3<float>& GetPosition() const;
	const Vector3<float>& GetDimension() const;
	float GetHP() const;
	float GetHorizontalRotation() const;
	float GetVerticalRotation() const;
	int GetAttackRange() const;
	float GetAttackSpeed() const;
	float GetArmor() const;
	float GetAttackDamage() const;
	const std::string& GetName() const;
	bool GetisAlive() const;
	bool GetisInAir() const;
	

protected:

	//Positionnement
	Vector3<float> m_pos;
	Vector3<float> m_dimension;
	Vector3<float> m_vitesse;
	float m_HorizontalRot;
	float m_VerticalRot;

	//Stat
	std::string m_Name;
	float m_health;
	float m_maxHealth;
	int m_AttackRange;
	float m_AttackSpeed;
	float m_Armor;
	float m_AttackDamage;

	//État
	bool m_isInAir;
	bool m_isAlive;

	//Timer
	sf::Clock m_cooldownAttackTimer;




};

#endif // MONSTER_H__

