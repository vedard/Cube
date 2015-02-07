#include "monster.h"

Monster::Monster() : Character()
{

	m_dimension = Vector3<float>(0.9, 2.6, 0.9);
	m_AttackRange = 2.2;
	m_AttackSpeed = 1.3;
	m_AttackDamage = 20;
	m_VerticalRot = 0;
	m_Armor = 1.1;
	m_target = NULL;
	m_isAlive = false;
}

Monster::~Monster()
{
}

void Monster::Move(World &world)
{
	if (m_isAlive)
	{
		m_vitesse.x = 0.05;
		m_vitesse.z = 0.05;

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
}

void Monster::Draw(Model3d &model, bool debugRange) const
{
	if (m_isAlive)
	{
		if (m_ClockAnimationDmg.getElapsedTime().asSeconds() < 0.1)
			model.Render(m_pos.x, m_pos.y, m_pos.z, m_HorizontalRot, m_VerticalRot, 1.f, 0.5f, 0.5f);
		else
			model.Render(m_pos.x, m_pos.y, m_pos.z, m_HorizontalRot, m_VerticalRot, 1.f, 1.f, 1.f);

		if (debugRange)
		{
			glPushMatrix();

			glTranslatef(m_pos.x, m_pos.y, m_pos.z);
			glRotatef(m_HorizontalRot, 0.f, 1.f, 0.f);

			glColor3f(1.f, 0.0f, 0.f);

			glBegin(GL_QUADS);

			glVertex3f(-m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
			glVertex3f(-m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);

			glVertex3f(-m_dimension.x / 2, 0, m_dimension.z / 2);
			glVertex3f(-m_dimension.x / 2, 0, -m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, 0, -m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, 0, m_dimension.z / 2);

			glVertex3f(-m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
			glVertex3f(-m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);

			glVertex3f(-m_dimension.x / 2, 0, m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, 0, m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, 0, -m_dimension.z / 2);
			glVertex3f(-m_dimension.x / 2, 0, -m_dimension.z / 2);

			glEnd();

			glBegin(GL_TRIANGLE_FAN);

			glVertex3f(0, m_dimension.y / 2, 0);
			for (int n = 0; n <= 10; ++n)
			{
				float angle = 2 * PI * n / 10.f;
				glVertex3f(sin(angle) * m_AttackRange, m_dimension.y / 2, cos(angle) * m_AttackRange);
			}

			glEnd();
			glPopMatrix();
		}

	}
}

void Monster::SetTarget(Character* target)
{
	m_target = target;
}

void Monster::GetDamage(float damage)
{
	m_ClockAnimationDmg.restart();

	Character::GetDamage(damage);
}