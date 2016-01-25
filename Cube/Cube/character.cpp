#include "character.h"


Character::Character() :
m_pos(0, 128, 0),
m_dimension(1.3f, 2.2f, 0.7f),
m_health(100),
m_AttackRange(2),
m_HorizontalRot(0.f),
m_VerticalRot(45.f),
m_vitesse(0, 0, 0),
m_AttackSpeed(0),
m_Armor(1),
m_cooldownAttackTimer(),
m_AttackDamage(30.0f),
m_isAlive(true)
{

	m_Name = "Character ";
	for (int i = 0; i < 5; i++)
		m_Name += std::to_string(std::rand() % 10);


}

Character::~Character()
{
}

void Character::Spawn(World &world, int x, int z)
{
	m_isAlive = true;
	m_health = 100;
	m_pos.x = (float)x;
	m_pos.y = CHUNK_SIZE_Y;
	m_pos.z = (float)z;

	while (!CheckCollision(world))
	{
		m_pos.y--;

		if (m_pos.y < -100)
		{
			m_pos.x = (float)x;
			m_pos.y = CHUNK_SIZE_Y;
			m_pos.z = (float)z;
			break;
		}
	}

	m_pos.y++;

	std::cout << m_Name << " spawned." << std::endl;
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

	int h = (int)ceil(m_dimension.y);
	int w = (int)ceil(m_dimension.x);
	int d = (int)ceil(m_dimension.z);

	/*
		dependament de la grandeur du personnage
		ont calcul un certain nombre de points
		et on vérifie si il n'y pas de block
		solide qui y touche
		*/
	for (int y = 0; y <= h; y++)
		for (int x = 0; x <= w; x++)
			for (int z = 0; z <= d; z++)
			{
				BlockType bt1 = world.BlockAt(
					m_pos.x - (m_dimension.x / w * x) + m_dimension.x / 2,
					m_pos.y + (m_dimension.y / h * y),
					m_pos.z - (m_dimension.z / d * z) + m_dimension.z / 2);

				//Si un des block n'est pas BTYPE_AIR OU BTYPE_WATER ou BTYPE_LAVA -> il y a collision
				if (bt1 != BTYPE_AIR && bt1 != BTYPE_WATER && bt1 != BTYPE_LAVA)
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

bool Character::Attack(Character * character, float damage)
{
	if (character != NULL)

		//Si on a la vitesse pour attaquer tout de suite
		if ((m_AttackSpeed == 0) ? true : (m_cooldownAttackTimer.getElapsedTime().asMilliseconds() > 1000 / m_AttackSpeed))

			//Si on est asser proche de la victime
			if (sqrtf(pow(character->GetPosition().x - m_pos.x, 2)
				+ pow((character->GetPosition().y + character->GetDimension().y / 2) - (m_pos.y + m_dimension.y / 2), 2)
				+ pow(character->GetPosition().z - m_pos.z, 2)) < m_AttackRange)
			{
				std::cout << m_Name << " attack " << character->GetName() << "." << std::endl;
				character->GetDamage(damage);
				m_cooldownAttackTimer.restart();
				return true;
			}

	return false;
}

bool Character::Attack(Character * character)
{
	return Attack(character, m_AttackDamage);
}

void Character::GetDamage(float damage)
{
	//Reduction par l'armur
	if (m_Armor > 0)
		damage /= m_Armor;

	//Toujours un minimun de 1 damange
	damage = (damage < 1) ? 1 : damage;

	m_health -= damage;

	std::cout << m_Name << " received " << damage << " damage." << std::endl;

	if (m_health <= 0)
	{
		m_isAlive = false;
		std::cout << m_Name << " died." << std::endl;
	}
}

void Character::Jump()
{
	if (!m_isInAir)
	{
		m_vitesse.y = -0.20f;
		m_isInAir = true;
	}
}

//Set

void Character::SetName(std::string name)
{
	m_Name = name;
}

//Get

const Vector3<float>& Character::GetPosition() const { return m_pos; }

const Vector3<float>& Character::GetDimension() const { return m_dimension; }

float Character::GetHP() const { return m_health; }

float Character::GetHorizontalRotation() const { return m_HorizontalRot; }

float Character::GetVerticalRotation() const { return m_VerticalRot; }

int Character::GetAttackRange() const { return m_AttackRange; }

float Character::GetAttackSpeed() const { return m_AttackSpeed; }

float Character::GetArmor() const { return m_Armor; }

float  Character::GetAttackDamage() const{ return m_AttackDamage; }

const std::string& Character::GetName() const{ return m_Name; }

bool Character::GetisAlive() const { return m_isAlive; }

bool Character::GetisInAir() const{ return m_isInAir; }