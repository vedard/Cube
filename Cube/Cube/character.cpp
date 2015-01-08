#include "character.h"


Character::Character() :
m_pos(0, 128, 0),
m_dimension(1.3, 2.2, 0.7),
m_health(100),
m_AttackRange(2),
m_HorizontalRot(0.f),
m_VerticalRot(90.f),
m_vitesse(0, 0, 0)
{
}


Character::~Character()
{
}

void Character::Spawn(World &world, int x, int z)
{
	m_health = 100;
	m_pos.x = x;
	m_pos.y = CHUNK_SIZE_Y;
	m_pos.z = z;

	while (!CheckCollision(world))
	{
		m_pos.y--;

		if (m_pos.y < -100)
		{
			m_pos.x = x;
			m_pos.y = CHUNK_SIZE_Y;
			m_pos.z = z;
			break;
		}
	}

	m_pos.y++;
}

void Character::Move(World &world)
{
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

bool Character::CheckCollision(World &world) const
{
	//Todo	-> simplifier cette fonction
	//		-> plus le character est gros plus il y a besoins de point
	int nbrEtage = ceil(m_dimension.y);
	BlockType * bt1 = new BlockType[nbrEtage + 1];
	BlockType * bt2 = new BlockType[nbrEtage + 1];
	BlockType * bt3 = new BlockType[nbrEtage + 1];
	BlockType * bt4 = new BlockType[nbrEtage + 1];
	BlockType * bt5 = new BlockType[nbrEtage + 1];
	BlockType * bt6 = new BlockType[nbrEtage + 1];
	BlockType * bt7 = new BlockType[nbrEtage + 1];
	BlockType * bt8 = new BlockType[nbrEtage + 1];
	BlockType * bt9 = new BlockType[nbrEtage + 1];


	for (int i = 0; i <= nbrEtage; i++)
	{
		//4 coin
		bt1[i] = world.BlockAt(m_pos.x - m_dimension.x / 2, m_pos.y + (m_dimension.y / nbrEtage * i), m_pos.z + m_dimension.z / 2);
		bt2[i] = world.BlockAt(m_pos.x + m_dimension.x / 2, m_pos.y + (m_dimension.y / nbrEtage * i) , m_pos.z + m_dimension.z / 2);
		bt3[i] = world.BlockAt(m_pos.x + m_dimension.x / 2, m_pos.y + (m_dimension.y / nbrEtage * i) , m_pos.z - m_dimension.z / 2);
		bt4[i] = world.BlockAt(m_pos.x - m_dimension.x / 2, m_pos.y + (m_dimension.y / nbrEtage * i) , m_pos.z - m_dimension.z / 2);

		//4 point entre les coins 
		bt5[i] = world.BlockAt(m_pos.x, m_pos.y + (m_dimension.y / nbrEtage * i), m_pos.z - m_dimension.z / 2);
		bt6[i] = world.BlockAt(m_pos.x, m_pos.y + (m_dimension.y / nbrEtage * i), m_pos.z - m_dimension.z / 2);
		bt7[i] = world.BlockAt(m_pos.x + m_dimension.x / 2, m_pos.y + (m_dimension.y / nbrEtage * i), m_pos.z);
		bt8[i] = world.BlockAt(m_pos.x - m_dimension.x / 2, m_pos.y + (m_dimension.y / nbrEtage * i), m_pos.z);

		//1 point au milieu
		bt9[i] = world.BlockAt(m_pos.x, m_pos.y + (m_dimension.y / nbrEtage) * i, m_pos.z);


		//Si un des block qui touche au joeur n'est pas BTYPE_AIR OU BTYPE_WATER -> il y a collision
		if ((bt1[i] != BTYPE_AIR && bt1[i] != BTYPE_WATER) ||
			(bt2[i] != BTYPE_AIR && bt2[i] != BTYPE_WATER) ||
			(bt3[i] != BTYPE_AIR && bt3[i] != BTYPE_WATER) ||
			(bt4[i] != BTYPE_AIR && bt4[i] != BTYPE_WATER) ||
			(bt5[i] != BTYPE_AIR && bt5[i] != BTYPE_WATER) ||
			(bt6[i] != BTYPE_AIR && bt6[i] != BTYPE_WATER) ||
			(bt7[i] != BTYPE_AIR && bt7[i] != BTYPE_WATER) ||
			(bt8[i] != BTYPE_AIR && bt8[i] != BTYPE_WATER) ||
			(bt9[i] != BTYPE_AIR && bt9[i] != BTYPE_WATER))
			return true;
	}

	return false;
}

bool  Character::CheckCollision(Character & character) const
{
	if ((m_pos.x - m_dimension.x / 2 >= character.GetPosition().x + character.GetDimension().x / 2)     // trop à droite
		|| (m_pos.x + m_dimension.x / 2 <= character.GetPosition().x - character.GetDimension().x / 2)				// trop à gauche
		|| (m_pos.y - m_dimension.y / 2 >= character.GetPosition().y + character.GetDimension().y / 2)	// trop en bas
		|| (m_pos.y + m_dimension.y / 2 <= character.GetPosition().y - character.GetDimension().y / 2)				// trop en haut 
		|| (m_pos.z - m_dimension.z / 2 >= character.GetPosition().z + character.GetDimension().z / 2)  // trop derrière
		|| (m_pos.z + m_dimension.z / 2 <= character.GetPosition().z - character.GetDimension().z / 2))				// trop devant
		return false;
	else
		return true;
}

void Character::Draw() const
{
	glPushMatrix();
	glTranslatef(m_pos.x, m_pos.y, m_pos.z);

	glRotatef(m_HorizontalRot, 0.f, 1.f, 0.f);


	glColor3f(1.f, 0.0f, 0.5f);

	glBegin(GL_QUADS);
	glVertex3f(-m_dimension.x / 2, 0, -m_dimension.z / 2);
	glVertex3f(-m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
	glVertex3f(m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
	glVertex3f(m_dimension.x / 2, 0, -m_dimension.z / 2);

	glVertex3f(m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
	glVertex3f(m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
	glVertex3f(m_dimension.x / 2, 0, m_dimension.z / 2);
	glVertex3f(m_dimension.x / 2, 0, -m_dimension.z / 2);

	glVertex3f(-m_dimension.x / 2, 0, m_dimension.z / 2);
	glVertex3f(m_dimension.x / 2, 0, m_dimension.z / 2);
	glVertex3f(m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
	glVertex3f(-m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);

	glVertex3f(-m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
	glVertex3f(-m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
	glVertex3f(-m_dimension.x / 2, 0, -m_dimension.z / 2);
	glVertex3f(-m_dimension.x / 2, 0, m_dimension.z / 2);

	glVertex3f(-m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
	glVertex3f(m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
	glVertex3f(m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
	glVertex3f(-m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);

	glVertex3f(-m_dimension.x / 2, 0, m_dimension.z / 2);
	glVertex3f(-m_dimension.x / 2, 0, -m_dimension.z / 2);
	glVertex3f(m_dimension.x / 2, 0, -m_dimension.z / 2);
	glVertex3f(m_dimension.x / 2, 0, m_dimension.z / 2);
	glEnd();

	glPopMatrix();
}
void Character::Attack(Character * character, int damage) const
{
	if (character != NULL)
		character->GetDamage(damage);
}

void Character::GetDamage(int damage)
{
	m_health -= damage;
}

const Vector3<float>& Character::GetPosition() const
{
	return m_pos;
}

const Vector3<float>& Character::GetDimension() const
{
	return m_dimension;
}

int Character::GetHP() const
{
	return m_health;
}

float Character::GetHorizontalRotation() const
{
	return m_HorizontalRot;
}

float Character::GetVerticalRotation() const
{
	return m_VerticalRot;
}
void Character::Jump()
{
	if (!m_isInAir)
	{
		m_vitesse.y = -0.20f;
		m_isInAir = true;
	}
}
