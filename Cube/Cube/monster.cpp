#include "monster.h"


Monster::Monster() : Character()
{
	m_texture.Load(TEXTURE_PATH "monster.png");

	if (!m_texture.IsValid())
		std::cerr << "Unable to load texture (" << TEXTURE_PATH "monster.png" << ")" << std::endl;

	m_dimension = Vector3<float>(2, 2.67, 2);
	m_AttackRange = 2.2;
	m_AttackSpeed = 1.3;
	m_AttackDamage = 20;
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

void Monster::Draw(bool debugRange) const
{

	if (m_isAlive)
	{
		m_texture.Bind();
		
		glPushMatrix();

		glTranslatef(m_pos.x, m_pos.y, m_pos.z);
		glRotatef(m_HorizontalRot, 0.f, 1.f, 0.f);
		if (debugRange)
		{
			glDisable(GL_TEXTURE_2D);
			glColor3f(1.f, 0.0f, 0.5f);

			glBegin(GL_TRIANGLES);

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
		}

		glEnable(GL_TEXTURE_2D);
		glColor3f(1.f, 1.0f, 1.0f);
		glBegin(GL_QUADS);

		glTexCoord2f(1, 1); glVertex3f(-m_dimension.x / 2, 0, 0);
		glTexCoord2f(0, 1); glVertex3f(m_dimension.x / 2, 0, 0);
		glTexCoord2f(0, 0); glVertex3f(m_dimension.x / 2, m_dimension.y, 0);
		glTexCoord2f(1, 0); glVertex3f(-m_dimension.x / 2, m_dimension.y, 0);

		glEnd();

		glPopMatrix();
	}
}

void Monster::SetTarget(Character* target)
{
	m_target = target;
}
