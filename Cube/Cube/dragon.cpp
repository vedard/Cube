#include "dragon.h"


Dragon::Dragon() :Animal(A_DRAGON), m_falling(true)
{
	m_maxHealth = 1000;
	m_dimension = Vector3<float>(3.0f, 4.6f, 4.8f);
	m_Name = "I AM FIRE, I AM DEATH!";
	isHurt = false;
	chillCount = 0;
	m_Armor = 10;
	m_AttackDamage = 25;
	m_AttackRange = 5;
};
Dragon::~Dragon()
{
};

void Dragon::Move(World &world)
{

	if (m_isAlive && !m_isDying)
	{
		m_vitesse.x = 0.15f;
		m_vitesse.z = 0.15f;

		if (m_ClockTarget.getElapsedTime().asSeconds() < m_timeNextTarget)
		{
			Vector3<float> deplacementVector = Vector3<float>(sin(m_HorizontalRot / 180 * PI), 0.f, cos(m_HorizontalRot / 180 * PI));
			deplacementVector.Normalize();

			//Avance en x
			m_pos.x += deplacementVector.x * m_vitesse.x;
			if (CheckCollision(world))
			{
				m_pos.x -= deplacementVector.x * m_vitesse.x;
				m_pos.y++;
			}
			//En z
			m_pos.z += deplacementVector.z * m_vitesse.z;
			if (CheckCollision(world))
			{
				m_pos.z -= deplacementVector.z * m_vitesse.z;
				m_pos.y++;
			}

			if (m_falling)
			{
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
					m_falling = false;
					m_pos.y += 20;
				}
				//Acceleration
				m_vitesse.y += 0.013f;
			}

		}
		else
		{
			m_HorizontalRot += rand() % 200 - 100;
			m_timeNextTarget = rand() % 10;
			m_ClockTarget.restart();
		}

	}
	//if (m_isAlive)
	//{
	//	if (isHurt)
	//	{
	//		m_vitesse.x = 0.11f;
	//		m_vitesse.z = 0.11f;

	//		//Si la cible est valide
	//		if (m_target)
	//		{
	//			//On attaque, si c'est pas possible on avance
	//			if (!Attack(m_target))
	//			{
	//				//Distance entre le monstre et sa cible
	//				Vector3<float> DeltaTarget(m_target->GetPosition().x - m_pos.x, (m_target->GetPosition().y + m_target->GetDimension().y / 2) - (m_pos.y + m_dimension.y / 2), m_target->GetPosition().z - m_pos.z);

	//				//On le place face a la cible
	//				m_HorizontalRot = ((atan2(DeltaTarget.x, DeltaTarget.z) * 180 / PI));

	//				//On avance pas si on est assez proche de la cible
	//				if (sqrtf(pow(DeltaTarget.x, 2) + pow(DeltaTarget.y, 2) + pow(DeltaTarget.z, 2)) > m_AttackRange)
	//				{
	//					Vector3<float> deplacementVector = Vector3<float>(DeltaTarget.x, 0, DeltaTarget.z);
	//					deplacementVector.Normalize();

	//					//Avance en x
	//					m_pos.x += deplacementVector.x * m_vitesse.x;
	//					if (CheckCollision(world))
	//					{
	//						m_pos.x -= deplacementVector.x * m_vitesse.x;
	//						Jump();
	//					}
	//					//En y
	//					m_pos.z += deplacementVector.z * m_vitesse.z;
	//					if (CheckCollision(world))
	//					{
	//						m_pos.z -= deplacementVector.z * m_vitesse.z;
	//						Jump();
	//					}
	//				}
	//				chillCount++;
	//				if (chillCount > 10000)
	//				{
	//					isHurt = false;
	//					chillCount = false;
	//				}
	//			}

	//		}

	//		//Chute
	//		m_pos.y -= m_vitesse.y;

	//		//Si collision
	//		if (CheckCollision(world))
	//		{

	//			//Si on a touche le sol 
	//			if (m_vitesse.y > 0)
	//				m_isInAir = false;

	//			//annule
	//			m_pos.y += m_vitesse.y;
	//			m_vitesse.y = 0;
	//		}
	//		else
	//			m_isInAir = true;

	//		//Acceleration
	//		m_vitesse.y += 0.013f;
	//	}
	//	else
	//	{
	//		Animal::Move(world);
	//	}
	//}
}

bool Dragon::GetDamage(float damage, bool ignoreArmor, bool godMode, Sound::ListeSons son, bool playonce)
{
	chillCount = 0;
	isHurt = true;
	return Animal::GetDamage(damage, ignoreArmor, godMode, son, playonce);
}

void Dragon::SetTarget(Character* target)
{
	m_target = target;
}


