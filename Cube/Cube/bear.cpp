#include "bear.h"


Bear::Bear() :Animal(A_BEAR)
{
	m_health = 200;
	m_dimension = Vector3<float>(1.5f, 2.3f, 2.4f);
	m_Name = "BigBadBear aka BBB";
	isHurt = false;
	chillCount = 0;
};
Bear::~Bear()
{
};

void Bear::Move(World &world)
{
	if (m_isAlive)
	{
		if (isHurt)
		{
			m_vitesse.x = 0.05f;
			m_vitesse.z = 0.05f;

			//Si la cible est valide
			if (m_target)
			{
				//On attaque, si c'est pas possible on avance
				if (!Attack(m_target))
				{
					//Distance entre le monstre et sa cible
					Vector3<float> DeltaTarget(m_target->GetPosition().x - m_pos.x, (m_target->GetPosition().y + m_target->GetDimension().y / 2) - (m_pos.y + m_dimension.y / 2), m_target->GetPosition().z - m_pos.z);

					//On le place face a la cible
					m_HorizontalRot = ((atan2(DeltaTarget.x, DeltaTarget.z) * 180 / PI));

					//On avance pas si on est assez proche de la cible
					if (sqrtf(pow(DeltaTarget.x, 2) + pow(DeltaTarget.y, 2) + pow(DeltaTarget.z, 2)) > m_AttackRange)
					{
						Vector3<float> deplacementVector = Vector3<float>(DeltaTarget.x, 0, DeltaTarget.z);
						deplacementVector.Normalize();

						//Avance en x
						m_pos.x += deplacementVector.x * m_vitesse.x;
						if (CheckCollision(world))
						{
							m_pos.x -= deplacementVector.x * m_vitesse.x;
							Jump();
						}
						//En y
						m_pos.z += deplacementVector.z * m_vitesse.z;
						if (CheckCollision(world))
						{
							m_pos.z -= deplacementVector.z * m_vitesse.z;
							Jump();
						}
					}
				}

			}

			//Chute
			m_pos.y -= m_vitesse.y;

			//Si collision
			if (CheckCollision(world))
			{

				//Si on a touche le sol 
				if (m_vitesse.y > 0)
					m_isInAir = false;

				//annule
				m_pos.y += m_vitesse.y;
				m_vitesse.y = 0;
			}
			else
				m_isInAir = true;

			//Acceleration
			m_vitesse.y += 0.013f;
		}
		else
		{
			Animal::Move(world);
		}
	}
}

bool Bear::GetDamage(float damage, bool ignoreArmor, bool godMode, Sound::ListeSons son, bool playonce)
{

	isHurt = true;
	return Animal::GetDamage(damage,ignoreArmor,godMode,son,playonce);
}

void Bear::SetTarget(Character* target)
{
	m_target = target;
}


