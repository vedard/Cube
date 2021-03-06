#include "chicken.h"

Chicken::Chicken() :Animal(A_CHICKEN), isHurt(false)
{
	m_maxHealth = 250;
	m_dimension = Vector3<float>(0.8f, 1.1f, 1.f);
	m_Name = "Chicken";
	m_deathSink = 8;
}

Chicken::~Chicken()
{
}

void Chicken::Move(World &world)
{
	if (m_isAlive)
	{
		if (isHurt)
		{
			if (!m_isDying)
			{
				m_vitesse.x = 0.05f;
				m_vitesse.z = 0.05f;

				//Si la cible est valide
				if (m_target)
				{

					Vector3<float> DeltaTarget(m_target->GetPosition().x - m_pos.x, (m_target->GetPosition().y + m_target->GetDimension().y / 2) - (m_pos.y + m_dimension.y / 2), m_target->GetPosition().z - m_pos.z);

					//On le place face a la cible
					m_HorizontalRot = ((atan2(-DeltaTarget.x, -DeltaTarget.z) * 180 / PI));

					//On avance pas si on est assez proche de la cibleeee
					Vector3<float> deplacementVector = Vector3<float>(-DeltaTarget.x, 0, -DeltaTarget.z);
					deplacementVector.Normalize();

					//Avance en x
					m_pos.x += deplacementVector.x * m_vitesse.x;
					if (CheckCollision(world))
					{
						m_pos.x -= deplacementVector.x * m_vitesse.x;
						Jump();
					}
					//En z
					m_pos.z += deplacementVector.z * m_vitesse.z;
					if (CheckCollision(world))
					{
						m_pos.z -= deplacementVector.z * m_vitesse.z;
						Jump();
					}

					chillCount++;
					if (chillCount > 800)
					{
						isHurt = false;
						chillCount = false;
					}

				}
			}
			

			Character::Move(world);
		}
		else
		{
			Animal::Move(world);
		}
	}

}

void Chicken::SetTarget(Character* target)
{
	m_target = target;
}

bool Chicken::GetDamage(float damage, bool ignoreArmor, bool godMode, Character* killer, Sound::ListeSons son, bool playonce)
{
	chillCount = 0;
	isHurt = true;
	return Animal::GetDamage(damage, ignoreArmor, godMode, killer, son, playonce);
}
