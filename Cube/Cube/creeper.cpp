#include "creeper.h"

Creeper::Creeper() :Monster(M_CREEPER)
{
	m_maxHealth = 130;
}
Creeper::~Creeper()
{

}

void Creeper::Explosion(World &world)
{
	std::vector<Vector3<int>> blocsDestroyed;

	// Remplir un vecteur de tous les blocs qui seront détruits (nombre changeant dépendant du rayon
	for (int x = 0; x < explosionRadius; x++)
	{
		for (int y = -explosionRadius; y < explosionRadius; y++)
		{
			for (int z = 0; z < explosionRadius; z++)
			{
				if ((x + z) <= (explosionRadius - abs(y)))
				{
					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x + x, (int)GetPosition().y + y, (int)GetPosition().z + z));
					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x + x, (int)GetPosition().y + y, (int)GetPosition().z - z));
					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x - x, (int)GetPosition().y + y, (int)GetPosition().z - z));
					blocsDestroyed.push_back(Vector3<int>((int)GetPosition().x - x, (int)GetPosition().y + y, (int)GetPosition().z + z));
				}
			}
		}
	}

	// Détruire tous les blocs du vecteur en trouvant leur chunk
	Vector3<int> chunkPos(m_pos.x / CHUNK_SIZE_X, 0, m_pos.z / CHUNK_SIZE_Z);
	for (int i = 0; i < blocsDestroyed.size(); i++)
	{
		int x = blocsDestroyed[i].x - (chunkPos.x * CHUNK_SIZE_X);
		int y = blocsDestroyed[i].y - (chunkPos.y * CHUNK_SIZE_Y);
		int z = blocsDestroyed[i].z - (chunkPos.z * CHUNK_SIZE_Z);

		if (x < CHUNK_SIZE_X && x >= 0 &&
			y < CHUNK_SIZE_Y && y >= 0 &&
			z < CHUNK_SIZE_Z && z >= 0)
			world.ChunkAt((float)chunkPos.x, (float)chunkPos.z)->RemoveBloc(x, y, z);
		else
		{
			chunkPos = Vector3<int>(blocsDestroyed[i].x / CHUNK_SIZE_X, 0, blocsDestroyed[i].z / CHUNK_SIZE_Z);
			i--;
		}
	}
}

void Creeper::SetExplosionRadius(int radius)
{
	if (radius > 0)
		explosionRadius = radius;
}

bool Creeper::Attack(Character * character)
{
	return Character::Attack(character);
	
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
			Character::CheckBlock(world);
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
			else
			{
				Explosion(world);
				if (!m_isDying)
					std::cout << m_Name << " received " << m_health << " damage." << std::endl;

				m_health = 0;

				
				m_deathTick.restart();
				m_isDying = true;

				DeathCheck();
			}

		}
	}

	Character::Move(world);
}