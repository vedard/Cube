#include "animal.h"


Animal::Animal(ANIMAL_TYPE typ)
{
	m_dimension = Vector3<float>(1.5f, 2.3f, 2.4f);
	m_AttackRange = (int)5.2;
	m_AttackSpeed = 1.1f;
	m_AttackDamage = 10;
	m_VerticalRot = 0;
	m_Armor = 0.5f;
	m_HorizontalRot = (float)(rand() % 180);
	m_isAlive = false;
	m_timeNextTarget = 3;
	type = typ;

}

Animal::~Animal()
{
}

void Animal::Move(World &world)
{
	if (m_isAlive && !m_isDying)
	{
		m_vitesse.x = 0.05f;
		m_vitesse.z = 0.05f;
		Vector3<float> deplacementVector = Vector3<float>(sin(m_HorizontalRot / 180 * PI), 0.f, cos(m_HorizontalRot / 180 * PI));
		deplacementVector.Normalize();
		if (m_ClockTarget.getElapsedTime().asSeconds() < m_timeNextTarget)
		{


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

				m_pos.z -= deplacementVector.z * m_vitesse.z;
			Jump();
		}
		if (world.BlockAt(m_pos.x, m_pos.y - 1, m_pos.z) == 28) // Si c'est un trampoline en dessous
		{
			if (m_vitesse.y > 0.10f)
			{
				if (m_nbsauttrampoline == 0)
				{
					multiplicateur = -1.01f;
				}
				m_nbsauttrampoline++;
				if (m_nbsauttrampoline >= MAX_TRAMPOLINE_JUMP)
				{
					multiplicateur += 0.025f;
				}
				m_vitesse.y *= multiplicateur;
				if (m_vitesse.y <= -0.60f)
				{
					m_vitesse.y = -0.60f;
				}
				if (m_vitesse.y >= 0)
				{
					m_vitesse.y = 0;
				}
			}
		}
		if (!m_isInAir && m_vitesse.y >= 0.0f)
		{
			m_nbsauttrampoline = 0;
		}
		m_pos.y += deplacementVector.y * m_vitesse.y;
	}
	else
	{
		m_HorizontalRot += rand() % 200 - 100;
		m_timeNextTarget = rand() % 10;
		m_ClockTarget.restart();
	}



	Character::Move(world);
}

void Animal::Draw(Model3d &model) const
{
	if (m_isAlive)
	{
		if (m_ClockAnimationDmg.getElapsedTime().asSeconds() < 0.1)
			model.Render(m_pos.x, m_pos.y, m_pos.z, m_HorizontalRot, m_VerticalRot, 1.f, 0.5f, 0.5f);
		else
			model.Render(m_pos.x, m_pos.y, m_pos.z, m_HorizontalRot, m_VerticalRot, 1.f, 1.f, 1.f);

		if (true)
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

bool Animal::GetDamage(float damage, bool ignoreArmor, bool godMode, Sound::ListeSons son, bool playonce)
{
	Jump();
	if (rand() % 100 > 66)
		Sound::Play(Sound::FLESH_IMPACT);
	return Character::GetDamage(damage, ignoreArmor, godMode, son, playonce);
}

ANIMAL_TYPE Animal::GetType()
{
	return type;
}

void Animal::SetTarget(Character* target)
{
	std::cout << "penis" << std::endl;
}
