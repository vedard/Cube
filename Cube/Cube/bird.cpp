#include "bird.h"

Bird::Bird() :Animal(A_COW), m_falling(true)
{
	m_maxHealth = 140;
	m_dimension = Vector3<float>(1.f, 0.45f, 1.f);
	m_Name = "Bird";
}

Bird::~Bird()
{
}

void Bird::Move(World &world)
{
	if (m_isAlive && !m_isDying)
	{
		m_vitesse.x = 0.05f;
		m_vitesse.z = 0.05f;

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
					m_pos.y += 10;
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
	else
	{
		Character::Move(world);
	}
}
