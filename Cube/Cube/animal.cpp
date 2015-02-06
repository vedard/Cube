#include "animal.h"


Animal::Animal()
{
	m_dimension = Vector3<float>(2, 2, 2);
	m_AttackRange = 5.2;
	m_AttackSpeed = 1.1;
	m_AttackDamage = 10;
	m_VerticalRot = 0;
	m_Armor = 0.5;
	m_HorizontalRot = rand() % 180;
	m_isAlive = false;
	m_target.x = 0;
}

Animal::~Animal()
{
}

void Animal::Move(World &world)
{
	if (m_isAlive)
	{
		m_vitesse.x = 0.05;
		m_vitesse.z = 0.05;

		//Distance entre le monstre et sa cible
		Vector3<float> DeltaTarget(m_target.x - m_pos.x, 0, m_target.z - m_pos.z);

		//On le place face a la cible
		m_HorizontalRot = ((atan2(DeltaTarget.x, DeltaTarget.z) * 180 / PI));

		//On avance pas si on est assez proche de la cible
		if (sqrtf(pow(DeltaTarget.x, 2) + pow(DeltaTarget.z, 2)) > m_AttackRange && m_target.x != 0)
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
			//En z
			m_pos.z += deplacementVector.z * m_vitesse.z;
			if (CheckCollision(world))
			{
				m_pos.z -= deplacementVector.z * m_vitesse.z;
				Jump();
			}
		}
		else
		{
			m_target = Vector3<float>(m_pos.x - 15 + rand() % 30, m_pos.y - 15 + rand() % 30, m_pos.z - 15 + rand() % 30);
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

void Animal::Draw(Model3d &model) const
{
	if (m_isAlive)
	{
		if (m_ClockAnimationDmg.getElapsedTime().asSeconds() < 0.1)
			model.Render(m_pos.x, m_pos.y, m_pos.z, m_HorizontalRot, m_VerticalRot, 1.f, 0.5f, 0.5f);
		else
			model.Render(m_pos.x, m_pos.y, m_pos.z, m_HorizontalRot, m_VerticalRot, 1.f, 1.f, 1.f);

		if (false)
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
			glPopMatrix();
		}
	}
}

void Animal::GetDamage(float damage)
{
	m_ClockAnimationDmg.restart();
	Jump();
	Character::GetDamage(damage);
}