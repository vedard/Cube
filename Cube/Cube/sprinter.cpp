#include "sprinter.h"

Sprinter::Sprinter() :Monster(M_SPRINTER)
{
	m_health = 100000;
	m_AttackSpeed = .5f;
	m_dimension = Vector3<float>(3.f, 4.1f, 1.f);
}
Sprinter::~Sprinter()
{

}

void Sprinter::Move(World &world)
{
	if (m_isAlive && !m_isDying)
	{
		m_vitesse.x = 0.09f;
		m_vitesse.z = 0.09f;

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
	}
	Character::Move(world);
}