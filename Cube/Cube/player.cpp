#include "player.h"
#include <iostream>
#include <algorithm>
#include <cmath>


Player::Player() :
Character(),
m_noClip(false),
m_sneaked(false),
m_running(false),
m_block(BTYPE_GRASS),
m_footUnderwater(false),
m_headUnderwater(false),
m_HeadShake(0)
{
	m_dimension = Vector3<float>(0.2f, 1.62f, 0.2f);
	m_VerticalRot = 0;
	m_health = 100;
	m_Armor = 1.3f;
	m_weapon = W_BLOCK;
	m_isAlive = false;
	
}

Player::~Player()
{
}

void Player::TurnLeftRight(float value)
{
	m_HorizontalRot += value;
}

void Player::TurnTopBottom(float value)
{
	m_VerticalRot += value;
	if (m_VerticalRot > 90)
		m_VerticalRot = 90;
	if (m_VerticalRot < -90)
		m_VerticalRot = -90;
}

void Player::Move(bool front, bool back, bool left, bool right, World &world)
{
	//Orientation du player en rad
	float orientationPlayer = m_HorizontalRot * PI / 180;
	//Multiplicateur de vitesse
	m_vitesse.x = 0.1f;
	m_vitesse.z = 0.1f;

	//Change la vittesse selon l'etat du player

	if (m_noClip)
	{
		m_vitesse.x = 1.2f;
		m_vitesse.z = 1.2f;
		m_vitesse.y = 1.2f;
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
	Vector3<float> directionVectorNoClip(cosf(PI / 2 * 3 + orientationPlayer) * (cosf(-m_VerticalRot * PI / 180)), sinf(-m_VerticalRot * PI / 180), sinf(PI / 2 * 3 + orientationPlayer) * (cosf(-m_VerticalRot * PI / 180)));
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
	//Si on bouge pas -> vitesse = 0
	if (!left && !right && !front && !back)
	{
		m_vitesse.x = 0;
		m_vitesse.z = 0;


		//m_HeadShake -= 0.01;
		//if (m_HeadShake <= 0)
		m_HeadShake = 0;
	}
	else if (!m_isInAir)
		m_HeadShake += 2.2f * m_vitesse.x;


	//Normalize les vecteur
	deplacementVector.Normalize();


	//Si no clip (pas de collision)
	if (m_noClip)
	{
		m_pos.y += deplacementVector.y * m_vitesse.y;
		m_pos.x += deplacementVector.x * m_vitesse.x;
		m_pos.z += deplacementVector.z * m_vitesse.z;
		m_isInAir = true;
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
				m_isInAir = false;

				//Degat de chute 
				if (m_vitesse.y > 0.40f)
					GetDamage(exp(m_vitesse.y * 6));
			}
			//annule
			m_pos.y += m_vitesse.y;
			m_vitesse.y = 0;
		}

		//Acceleration
		m_vitesse.y += (m_footUnderwater) ? 0.002f : 0.013f;

	}

	//Si le player est dans l'eau
	CheckUnderwater(world);

	if (m_footUnderwater)
	{


		if (m_vitesse.y > 0.08f)
			m_vitesse.y = 0.08f;
	}

	
}

void Player::CheckUnderwater(World &world)
{
	BlockType bt1 = world.BlockAt(m_pos.x, m_pos.y + m_dimension.y, m_pos.z);

	if (bt1 == BTYPE_WATER)
		m_headUnderwater = true;
	else
		m_headUnderwater = false;

	bt1 = world.BlockAt(m_pos.x, m_pos.y + m_dimension.y / 1.5f, m_pos.z);

	if (bt1 == BTYPE_WATER)
		m_footUnderwater = true;
	else
		m_footUnderwater = false;
}

void Player::ApplyRotation() const
{
	glMatrixMode(GL_MODELVIEW);
	glRotatef(m_VerticalRot, 1, 0, 0);
	glRotatef(m_HorizontalRot, 0, 1, 0);

}

void Player::ApplyTranslation()
{

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(-m_pos.x, -(m_pos.y + m_dimension.y), -m_pos.z);

	//Head shake a chaque pas
	if (m_vitesse.x != 0 && !m_noClip)
	{
		glTranslatef(0.f, m_vitesse.x / 2.2f * sin(m_HeadShake), 0.0f);

	}

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
	if (!m_noClip && !m_isInAir)
		m_running = running;
}

BlockType Player::GetBlock() const { return m_block; }

int Player::GetWeapon() const { return m_weapon; }

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

void Player::SetWeapon(int mode)
{
	if (mode >= 0 && mode <= 3)
		m_weapon = mode;
}

void Player::Jump()
{
	if (!m_isInAir && !m_footUnderwater && Tool::EqualWithEpsilon<float>(m_vitesse.y,0, 0.20))
	{
		m_vitesse.y = -0.20f;
		m_isInAir = true;
	}
	else if (m_footUnderwater && !m_headUnderwater)
		m_vitesse.y = -0.002f;
	else if (m_footUnderwater)
		m_vitesse.y = -0.09f;
}

bool Player::Underwater() const { return m_headUnderwater; }

