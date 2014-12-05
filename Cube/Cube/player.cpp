#include "player.h"
#include <iostream>
#include <algorithm>
#include <cmath>


Player::Player(float posX, float posY, float posZ, float rotX, float rotY) : 
m_pos(posX, posY, posZ),
m_dimension(0.2, 1.62, 0.2), 
m_rotX(rotX), 
m_rotY(rotY),
m_vitesse(4), 
m_noClip(false), 
m_sneaked(false), 
m_vitesseY(0), 
m_health(100), 
m_running(false),
m_block(BTYPE_GRASS)
{

}

Player::~Player()
{

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

void Player::Move(bool front, bool back, bool left, bool right, float elapsedTime, World &world)
{
	//Orientation du player en rad
	float orientationPlayer = m_rotX * PI / 180;
	//Multiplicateur de vitesse
	float multiplieur = m_vitesse * elapsedTime;

	//Change la vittesse selon l'etat du player
	if (m_sneaked)
		multiplieur *= 0.7;
	else if (m_noClip)
		multiplieur *= 6;
	else if (m_running)
		multiplieur *= 1.4;

	//Deplacement Avant/Arriere
	Vector3<float> directionVector(cos(PI / 2 * 3 + orientationPlayer), 0, sin(PI / 2 * 3 + orientationPlayer));
	Vector3<float> directionVectorNoClip(cos(PI / 2 * 3 + orientationPlayer) * (cos(-m_rotY * PI / 180)), sin(-m_rotY * PI / 180), sin(PI / 2 * 3 + orientationPlayer) * (cos(-m_rotY * PI / 180)));
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
		m_pos.y += deplacementVector.y * multiplieur;
		m_pos.x += deplacementVector.x * multiplieur;
		m_pos.z += deplacementVector.z * multiplieur;
	}

	else
	{
		//Deplacement en X
		m_pos.x += deplacementVector.x * multiplieur;
		//Si collision, on annule
		if (CheckCollision(world))
			m_pos.x -= deplacementVector.x * multiplieur;

		//Deplacement en Z
		m_pos.z += deplacementVector.z * multiplieur;
		//Si collision, on annule
		if (CheckCollision(world))
			m_pos.z -= deplacementVector.z * multiplieur;

	}

	//Gravité
	if (!m_noClip)
	{
		//Chute
		m_pos.y -= m_vitesseY;

		//Si collision
		if (CheckCollision(world))
		{
			//Si on a touche le sol 
			if (m_vitesseY > 0)
				m_air = false;

			//annule
			m_pos.y += m_vitesseY;
			m_vitesseY = 0;
		}
		else
			m_air = true;

		//Acceleration
		m_vitesseY += 0.013;
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

		//Si un des block qui touche au joeur n'est pas BTYPE_AIR -> il y a collision
	if (bt1 != BTYPE_AIR || bt2 != BTYPE_AIR || bt3 != BTYPE_AIR ||
		bt4 != BTYPE_AIR || bt5 != BTYPE_AIR || bt6 != BTYPE_AIR ||
		bt7 != BTYPE_AIR || bt8 != BTYPE_AIR || bt9 != BTYPE_AIR ||
		bt10 != BTYPE_AIR || bt11 != BTYPE_AIR || bt12 != BTYPE_AIR)
		return true;


	return false;

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
		glTranslatef(0, +0.2, 0);
}

void Player::ToggleNoClip()
{
	if (m_noClip)
	{
		m_noClip = false;
	}
	else
	{
		m_noClip = true;
		m_vitesseY = 0;
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
	if (!m_air)
	{
		m_vitesseY = -0.20;
		m_air = true;
	}
}

int Player::GetHP() const
{
	return m_health;
}
