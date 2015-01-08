#include "monster.h"


Monster::Monster() : Character()
{
	m_texture.Load(TEXTURE_PATH "monster.png");

	if (!m_texture.IsValid())
		std::cerr << "Unable to load texture (" << TEXTURE_PATH "monster.png" << ")" << std::endl;

	m_dimension = Vector3<float>(1.3, 2.2, 1.3);
}


Monster::~Monster()
{
}

void Monster::Move(World &world, Player &player)
{
	m_vitesse.x = 0.05;
	m_vitesse.z = 0.05;

	m_target = player.GetPosition();


	//On le place face au joueur
	m_HorizontalRot = ((atan2(m_target.x - m_pos.x, m_target.z - m_pos.z) * 180 / PI));
	Vector3<float> deplacementVector;
	if (sqrtf(pow(m_target.x - m_pos.x, 2) + pow(m_target.z - m_pos.z, 2)) > m_dimension.x/2)
		deplacementVector = Vector3<float>(m_target.x - m_pos.x, 0.f, m_target.z - m_pos.z);


	deplacementVector.Normalize();

	m_pos.x += deplacementVector.x * m_vitesse.x;
	if (CheckCollision(world))
	{
		m_pos.y++;
		if (CheckCollision(world))
		{
			m_pos.x -= deplacementVector.x * m_vitesse.x;
			m_pos.y--;
		}
	}
	m_pos.z += deplacementVector.z * m_vitesse.z;
	if (CheckCollision(world))
	{
		m_pos.y++;
		if (CheckCollision(world))
		{
			m_pos.z -= deplacementVector.z * m_vitesse.z;
			m_pos.y--;
		}
	}

	//Chute
	m_pos.y -= m_vitesse.y;

	//Si collision
	if (CheckCollision(world))
	{
		//annule
		m_pos.y += m_vitesse.y;
		m_vitesse.y = 0;
	}

	//Acceleration
	m_vitesse.y += 0.013f;
}

void Monster::Draw() const
{
	m_texture.Bind();

	glPushMatrix();
	glTranslatef(m_pos.x, m_pos.y, m_pos.z);

	glRotatef(m_HorizontalRot, 0.f, 1.f, 0.f);

	//glColor4f(1.f, 0.0f, 0.5f,0.2f);

	//glBegin(GL_QUADS);
	//glVertex3f(-m_dimension.x / 2, 0, -m_dimension.z / 2);
	//glVertex3f(-m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
	//glVertex3f(m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
	//glVertex3f(m_dimension.x / 2, 0, -m_dimension.z / 2);

	//glVertex3f(m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
	//glVertex3f(m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
	//glVertex3f(m_dimension.x / 2, 0, m_dimension.z / 2);
	//glVertex3f(m_dimension.x / 2, 0, -m_dimension.z / 2);

	//glVertex3f(-m_dimension.x / 2, 0, m_dimension.z / 2);
	//glVertex3f(m_dimension.x / 2, 0, m_dimension.z / 2);
	//glVertex3f(m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
	//glVertex3f(-m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);

	//glVertex3f(-m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
	//glVertex3f(-m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
	//glVertex3f(-m_dimension.x / 2, 0, -m_dimension.z / 2);
	//glVertex3f(-m_dimension.x / 2, 0, m_dimension.z / 2);

	//glVertex3f(-m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
	//glVertex3f(m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
	//glVertex3f(m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
	//glVertex3f(-m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);

	//glVertex3f(-m_dimension.x / 2, 0, m_dimension.z / 2);
	//glVertex3f(-m_dimension.x / 2, 0, -m_dimension.z / 2);
	//glVertex3f(m_dimension.x / 2, 0, -m_dimension.z / 2);
	//glVertex3f(m_dimension.x / 2, 0, m_dimension.z / 2);
	//glEnd();

	

	glBegin(GL_QUADS);


	glTexCoord2f(1, 1); glVertex3f(-m_dimension.x / 2, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(m_dimension.x / 2, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(m_dimension.x / 2, m_dimension.y, 0);
	glTexCoord2f(1, 0); glVertex3f(-m_dimension.x / 2, m_dimension.y, 0);


	glEnd();

	glPopMatrix();
}
