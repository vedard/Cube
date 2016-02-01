#include "gun.h"


Gun::Gun()
{
	m_bullets = new Bullet[MAX_BULLET];
	bool m_isAutomatic = false;
	float m_cooldown = 0.01f;
	m_recoilByBullet = 0.09;
	m_recoilTotal = 0.0f;
}


Gun::~Gun()
{
}


void Gun::Init(std::string modelPath, std::string texturePath, int sound, bool isAuto, float RPM, float damage, float aimHeight)
{
	m_sound = sound;
	m_damage = damage;
	m_isAutomatic = isAuto;
	m_cooldown = 1 / (RPM / 60);
	m_model.LoadOBJ(modelPath, texturePath);
	m_aimingHeight = aimHeight;
}

void Gun::Draw(float x, float y, float z, float rotX, float rotY) const
{

	//Permet de render le model 3d de l'Arme a la bonne position (en bas a droite de l'ecran)
	if(m_isAiming)
		m_model.Render(
			(float)(x - (cos(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180 + PI / 2)) * (0.7 - m_recoilTotal) + cos(rotX * PI / 180) * (0.0) + (cos(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180)) * m_aimingHeight),
			(float)(y + cos(rotY * PI / 180 + PI / 2 )* (0.7 - m_recoilTotal) - cos(rotY * PI / 180) * m_aimingHeight),
			(float)(z - (sin(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180 + PI / 2)) * (0.7 - m_recoilTotal) + sin(rotX * PI / 180) * (0.0) + (sin(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180)) * m_aimingHeight),
			-rotX, -rotY, 1, 1, 1);
	else
		m_model.Render(
			(float)(x - (cos(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180 + PI / 2)) * (1 - m_recoilTotal) + cos(rotX * PI / 180) * (0.4) + (cos(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180)) * 0.5),
			(float)(y + cos(rotY * PI / 180 + PI / 2 )* (1 - m_recoilTotal) - cos(rotY * PI / 180) * 0.5),
			(float)(z - (sin(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180 + PI / 2)) * (1 - m_recoilTotal) + sin(rotX * PI / 180) * (0.4) + (sin(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180)) * 0.5),
			-rotX, -rotY, 1, 1, 1);
}

void Gun::Update() 
{
	m_recoilTotal /= 1.2;
	if (m_recoilTotal < 0)
		m_recoilTotal = 0;

}

bool Gun::Shoot(float x, float y, float z, float rotX, float rotY)
{
	if (m_timerCooldown.getElapsedTime().asSeconds() > m_cooldown)
	{
		m_timerCooldown.restart();
		for (int i = 0; i < MAX_BULLET; i++)
			if (!m_bullets[i].GetIsActive())
			{
				if(m_isAiming)
					m_bullets[i].Init(x, y, z, rotY + ((rand() % 40) - 20) * 20 / 400, rotX + ((rand() % 40) - 20) * 20 / 400, m_damage);
				else
					m_bullets[i].Init(x, y, z, rotY + ((rand() % 100) - 50) * 20 / 400, rotX + ((rand() % 100) - 50) * 20 / 400, m_damage);
				m_recoilTotal += m_recoilByBullet;
				Sound::Play(m_sound);
				return true;
			}
	}
	else
		return false;
	return false;
}

void Gun::EnableAiming()
{
	m_isAiming = true;
}

void Gun::DisableAiming()
{
	m_isAiming = false;
}

const bool Gun::isAiming()
{
	return m_isAiming;
}

Bullet* Gun::GetBullets()
{
	return m_bullets;
}

const bool Gun::GetIsAuto() const
{
	return m_isAutomatic;

}
