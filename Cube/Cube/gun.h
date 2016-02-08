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
	void InitRessource(std::string modelPath, std::string texturePath, int sound);
	void InitStat(bool isAuto, float RPM, float damage, float aimHeight);
	void Draw(float x, float y, float z, float rotX, float rotY) const;
	void Update();
	bool Shoot(float x, float y, float z, float rotX, float rotY);
	void EnableAiming();
	void DisableAiming();
	const bool isAiming();
	Bullet* GetBullets();
	const bool GetIsAuto() const;


private:
	Model3d m_model;
	Bullet * m_bullets;

	//Stat de l'arme
	bool m_isAutomatic;
	float m_cooldown;
	float m_damage;
	float m_recoilByBullet;
	float m_aimingHeight;

	//Timer
	sf::Clock m_timerCooldown;

	float m_recoilTotal;

	int m_sound;
	bool m_isAiming;

};

#endif

