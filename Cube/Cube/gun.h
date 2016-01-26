#ifndef GUN_H__
#define GUN_H__

#include "bullet.h"
#include "model3d.h"
#include "sound.h"
#include "item.h"

class Gun
{

public:
	Gun();
	~Gun();
	void Init(std::string modelPath, std::string texturePath, int sound, bool isAuto, float RPM, float damage);
	void Draw(float x, float y, float z, float rotX, float rotY) const;
	bool Shoot(float x, float y, float z, float rotX, float rotY);
	Bullet* GetBullets();
	const bool GetIsAuto() const;


private:
	Model3d m_model;
	Bullet * m_bullets;

	//Stat de l'arme
	bool m_isAutomatic;
	float m_cooldown;
	float m_damage;

	//Timer
	sf::Clock m_timerCooldown;

	int m_sound;

};

#endif

