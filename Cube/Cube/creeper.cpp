#include "creeper.h"

Creeper::Creeper() :Monster(M_CREEPER)
{
	m_health = 130;
}
Creeper::~Creeper()
{

}

std::vector<Vector3<int>> Creeper::Explosion()
{
	std::vector<Vector3<int>> blocsDestroyed;

	for (int x = 0; x < explosionRadius; x++)
	{
		for (int y = -explosionRadius; y < explosionRadius; y++)
		{
			for (int z = 0; z < explosionRadius; z++)
			{
				if ((x + z) <= (explosionRadius - abs(y)))
				{
					// Pour les 4 quartiles
					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x + x, (int)GetPosition().y + y, (int)GetPosition().z + z));
					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x + x, (int)GetPosition().y + y, (int)GetPosition().z - z));
					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x - x, (int)GetPosition().y + y, (int)GetPosition().z - z));
					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x - x, (int)GetPosition().y + y, (int)GetPosition().z + z));
				}
			}
		}
	}

	return blocsDestroyed;
}

bool Creeper::Attack(Character * character)
{
	if (!Character::Attack(character));
	{
		Explosion();
		return false;
	}
	return true;
}

void Creeper::Move(World &world)
{
	if (m_isAlive && !m_isDying)
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
	}

	Character::Move(world);
}



