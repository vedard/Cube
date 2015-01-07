#include "player.h"
#include <iostream>
#include <algorithm>
#include <cmath>


Player::Player() :
m_pos(0, 128, 0),
m_dimension(0.2f, 1.62f, 0.2f),
m_rotX(0),
m_rotY(0),
m_vitesse(0.1f,0,0),
m_noClip(false),
m_sneaked(false),

m_health(100),
m_running(false),
m_block(BTYPE_GRASS),
m_footUnderwater(false),
m_headUnderwater(false)
{

}

Player::~Player()
{

}

void Player::Spawn(World &world)
{
	m_health = 100;
	m_pos.x = WORLD_SIZE*CHUNK_SIZE_X / 2;
	m_pos.y = CHUNK_SIZE_Y;
	m_pos.z = WORLD_SIZE*CHUNK_SIZE_Z / 2;

	while (!CheckCollision(world))
	{
		m_pos.y--;

		if (m_pos.y < -100)
		{
			m_pos.x = WORLD_SIZE*CHUNK_SIZE_X / 2;
			m_pos.y = CHUNK_SIZE_Y;
			m_pos.z = WORLD_SIZE*CHUNK_SIZE_Z / 2;
			break;
		}
	}

	m_pos.y++;
}

void Player::TurnLeftRight(float value)
{
	m_rotX += value;
}

void Player::TurnTopBottom(float value)
{
	m_rotY += value;
	if (m_rotY > 90)
		m_rotY = 90;
	if (m_rotY < -90)
		m_rotY = -90;


}

void Player::Move(bool front, bool back, bool left, bool right, World &world)
{
	//Orientation du player en rad
	float orientationPlayer = m_rotX * PI / 180;
	//Multiplicateur de vitesse
	m_vitesse.x = 0.1;
	m_vitesse.z = 0.1;

	//Change la vittesse selon l'etat du player

	if (m_noClip)
	{
		m_vitesse.x = 1.2;
		m_vitesse.z = 1.2;
		m_vitesse.y = 1.2;
	}
	else
	{
		if (m_sneaked)
		{
			m_vitesse.x *= 0.7f;
			m_vitesse.z *= 0.7f;
		
		}
		else if (m_running)
		{
			m_vitesse.x *= 1.4f;
			m_vitesse.z *= 1.4f;

		}
		if (m_footUnderwater)
		{
			m_vitesse.x *= 0.6f;
			m_vitesse.z *= 0.6f;
		}
	}

	//Deplacement Avant/Arriere
	Vector3<float> directionVector(cosf(PI / 2.f * 3.f + orientationPlayer), 0.f, sinf(PI / 2.f * 3.f + orientationPlayer));
	Vector3<float> directionVectorNoClip(cosf(PI / 2 * 3 + orientationPlayer) * (cosf(-m_rotY * PI / 180)), sinf(-m_rotY * PI / 180), sinf(PI / 2 * 3 + orientationPlayer) * (cosf(-m_rotY * PI / 180)));
	//Deplacement Gauche/Droite
	Vector3<float> rightVector = directionVector.Cross(Vector3<float>(0, 1, 0));
	//Deplacement Total
	Vector3<float> deplacementVector(0, 0, 0);

	//Selon les touches appuie on modifie le vecteur de deplacement
	if (front) {
		if (m_noClip)
			deplacementVector += directionVectorNoClip;
		else
			deplacementVector += directionVector;
	}
	else if (back) {
		if (m_noClip)
			deplacementVector -= directionVectorNoClip;
		else
			deplacementVector -= directionVector;
	}
	if (right) {
		deplacementVector += rightVector;
	}
	else if (left) {
		deplacementVector -= rightVector;
	}

	//Normalize les vecteur
	deplacementVector.Normalize();
	rightVector.Normalize();

	//Si no clip (pas de collision)
	if (m_noClip)
	{
		m_pos.y += deplacementVector.y * m_vitesse.y;
		m_pos.x += deplacementVector.x * m_vitesse.x;
		m_pos.z += deplacementVector.z * m_vitesse.z;
	}

	else
	{
		//Deplacement en X
		m_pos.x += deplacementVector.x * m_vitesse.x;
		//Si collision, on annule
		if (CheckCollision(world))
			m_pos.x -= deplacementVector.x * m_vitesse.x;

		//Deplacement en Z
		m_pos.z += deplacementVector.z * m_vitesse.z;
		//Si collision, on annule
		if (CheckCollision(world))
			m_pos.z -= deplacementVector.z * m_vitesse.z;

	
		//Deplacement en Y (Gravité)
		m_pos.y -= m_vitesse.y;

		//Si collision
		if (CheckCollision(world))
		{
			//Si on a touche le sol 
			if (m_vitesse.y > 0)
			{
				m_air = false;

				//Degat de chute 
				if (m_vitesse.y > 0.40f)
					Hurt((int) exp(m_vitesse.y * 6));
			}
			//annule
			m_pos.y += m_vitesse.y;
			m_vitesse.y = 0;
		}
		else
			m_air = true;

		//Acceleration
		m_vitesse.y += (m_footUnderwater)? 0.002f : 0.013f;
		
	}

	//Si le player est mort
	if (m_health <= 0)
	{
		Spawn(world);
	}

	//Si le player est dans l'eau
	CheckUnderwater(world);

	if (m_footUnderwater)
	{
		

		if (m_vitesse.y > 0.08f)
			m_vitesse.y = 0.08f;
	}
}

bool Player::CheckCollision(World &world) const
{

	//4 point au pieds du player
	BlockType bt1 = world.BlockAt(m_pos.x - m_dimension.x / 2, m_pos.y, m_pos.z + m_dimension.z / 2);
	BlockType bt2 = world.BlockAt(m_pos.x + m_dimension.x / 2, m_pos.y, m_pos.z + m_dimension.z / 2);
	BlockType bt3 = world.BlockAt(m_pos.x + m_dimension.x / 2, m_pos.y, m_pos.z - m_dimension.z / 2);
	BlockType bt4 = world.BlockAt(m_pos.x - m_dimension.x / 2, m_pos.y, m_pos.z - m_dimension.z / 2);

	//4 point au milieu du player

	BlockType bt5 = world.BlockAt(m_pos.x - m_dimension.x / 2, m_pos.y + m_dimension.y / 2, m_pos.z + m_dimension.z / 2);
	BlockType bt6 = world.BlockAt(m_pos.x + m_dimension.x / 2, m_pos.y + m_dimension.y / 2, m_pos.z + m_dimension.z / 2);
	BlockType bt7 = world.BlockAt(m_pos.x + m_dimension.x / 2, m_pos.y + m_dimension.y / 2, m_pos.z - m_dimension.z / 2);
	BlockType bt8 = world.BlockAt(m_pos.x - m_dimension.x / 2, m_pos.y + m_dimension.y / 2, m_pos.z - m_dimension.z / 2);

	//4 point au yeux du player
	BlockType bt9 = world.BlockAt(m_pos.x - m_dimension.x / 2, m_pos.y + m_dimension.y, m_pos.z + m_dimension.z / 2);
	BlockType bt10 = world.BlockAt(m_pos.x + m_dimension.x / 2, m_pos.y + m_dimension.y, m_pos.z + m_dimension.z / 2);
	BlockType bt11 = world.BlockAt(m_pos.x + m_dimension.x / 2, m_pos.y + m_dimension.y, m_pos.z - m_dimension.z / 2);
	BlockType bt12 = world.BlockAt(m_pos.x - m_dimension.x / 2, m_pos.y + m_dimension.y, m_pos.z - m_dimension.z / 2);



	//Si un des block qui touche au joeur n'est pas BTYPE_AIR OU BTYPE_WATER -> il y a collision
	if ((bt1 != BTYPE_AIR && bt1 != BTYPE_WATER)||
		(bt2 != BTYPE_AIR && bt2 != BTYPE_WATER) || 
		(bt3 != BTYPE_AIR && bt3 != BTYPE_WATER) || 
		(bt4 != BTYPE_AIR && bt4 != BTYPE_WATER) || 
		(bt5 != BTYPE_AIR && bt5 != BTYPE_WATER) || 
		(bt6 != BTYPE_AIR && bt6 != BTYPE_WATER) || 
		(bt7 != BTYPE_AIR && bt7 != BTYPE_WATER) || 
		(bt8 != BTYPE_AIR && bt8 != BTYPE_WATER) || 
		(bt9 != BTYPE_AIR && bt9 != BTYPE_WATER) || 
		(bt10 != BTYPE_AIR && bt10 != BTYPE_WATER) || 
		(bt11 != BTYPE_AIR && bt11 != BTYPE_WATER) || 
		(bt12 != BTYPE_AIR && bt12 != BTYPE_WATER))
		return true;
		


	return false;

}

void Player::CheckUnderwater(World &world)
{
	BlockType bt1 = world.BlockAt(m_pos.x , m_pos.y + m_dimension.y, m_pos.z );

	if (bt1 == BTYPE_WATER )
		m_headUnderwater = true;
	else
		m_headUnderwater = false;

	bt1 = world.BlockAt(m_pos.x, m_pos.y + m_dimension.y/1.5f, m_pos.z);

	if (bt1 == BTYPE_WATER)
		m_footUnderwater = true;
	else
		m_footUnderwater = false;
}

void Player::ApplyRotation() const
{
	glMatrixMode(GL_MODELVIEW);
	glRotatef(m_rotY, 1, 0, 0);
	glRotatef(m_rotX, 0, 1, 0);

}

void Player::ApplyTranslation() const
{

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(-m_pos.x, -(m_pos.y + m_dimension.y), -m_pos.z);

	//Si on est baisse 
	if (m_sneaked)
		glTranslatef(0.f, 0.2f, 0.f);
}

void Player::ToggleNoClip()
{
	if (m_noClip)
	{
		m_noClip = false;
		m_vitesse.y = 0;
	}
	else
	{
		m_noClip = true;
		m_vitesse.y = 0;
	}
}

void Player::SetSneak(bool sneak)
{
	//On peut pas se baisser en mode noclip
	if (!m_noClip)
		m_sneaked = sneak;
}

void Player::SetRunning(bool running)
{
	if (!m_noClip && !m_air)
		m_running = running;
}

BlockType Player::GetBlock() const
{
	return m_block;
}

void Player::SetBlock(int direction)
{
	if (direction < 0)
		m_block--;

	else if (direction > 0)
		m_block++;

	if (m_block == 0)
		m_block = NUMBER_OF_BLOCK - 1;

	else if (m_block == NUMBER_OF_BLOCK)
		m_block = 1;
}

Vector3<float> Player::Position() const
{
	return m_pos;
}

Vector3<float> Player::GetDimension() const
{
	return m_dimension;
}

void Player::Jump()
{
	if (!m_air && !m_footUnderwater)
	{
		m_vitesse.y = -0.20f;
		m_air = true;
	}
	else if (m_footUnderwater && !m_headUnderwater)
		m_vitesse.y = -0.002f;
	else if (m_footUnderwater)
		m_vitesse.y = -0.09f;
}

void Player::Hurt(int damage)
{
	/*
		Damage reduction (armor, spell and etc...)
		*/

	m_health -= damage;


}

int Player::GetHP() const
{
	return m_health;
}

bool Player::Underwater() const
{
	return m_headUnderwater;
}
