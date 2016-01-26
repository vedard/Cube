#include "gun.h"


Gun::Gun()
{
	m_bullets = new Bullet[MAX_BULLET];
	bool m_isAutomatic = false;
	float m_cooldown = 0.01f;
}


Gun::~Gun()
{
}

void Gun::Init(std::string modelPath, std::string texturePath, int sound, bool isAuto, float RPM, float damage)
{
	m_sound = sound;
	m_damage = damage;
	m_isAutomatic = isAuto;
	m_cooldown = 1 / (RPM / 60);
	m_model.LoadOBJ(modelPath, texturePath);
}

void Gun::Draw(float x, float y, float z, float rotX, float rotY) const
{
	//Permet de render le model 3d de l'Arme a la bonne position (en bas a droite de l'ecran)
	m_model.Render(
		(float)(x - (cos(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180 + PI / 2)) + cos(rotX * PI / 180) * 0.4 + (cos(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180)) * 0.5),
		(float)(y + cos(rotY * PI / 180 + PI / 2) - cos(rotY * PI / 180) * 0.5),
		(float)(z - (sin(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180 + PI / 2)) + sin(rotX * PI / 180) * 0.4 + (sin(rotX * PI / 180 + PI / 2) * sin(rotY * PI / 180)) * 0.5),
		-rotX, -rotY, 1, 1, 1);
}

bool Gun::Shoot(float x, float y, float z, float rotX, float rotY)
{
	if (m_timerCooldown.getElapsedTime().asSeconds() > m_cooldown)
	{
		m_timerCooldown.restart();
		for (int i = 0; i < MAX_BULLET; i++)
			if (!m_bullets[i].GetIsActive())
			{
				m_bullets[i].Init(x, y, z, rotY + (rand() % 50 - 25) * 20 / 400, rotX + (rand() % 50 - 25) * 20 / 400, m_damage);
				Sound::Play(m_sound);
				return true;
			}
	}
	else
		return false;
	return false;
}

Bullet* Gun::GetBullets()
{
	return m_bullets;
}

const bool Gun::GetIsAuto() const
{
	return m_isAutomatic;
}

