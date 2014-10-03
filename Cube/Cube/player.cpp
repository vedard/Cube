#include "player.h"
#include <iostream>
#include <algorithm>
#include <cmath>


Player::Player(float posX, float posY, float posZ, float rotX, float rotY) : m_posX(posX), m_posY(posY), m_posZ(posZ), m_rotX(rotX), m_rotY(rotY), m_vitesse(4), m_noClip(false)
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

void Player::Move(bool front, bool back, bool left, bool right, bool shift, float elapsedTime)
{
	float orientationPlayer = m_rotX * PI / 180;
	float multiplieur = (shift) ? m_vitesse * 1.7 * elapsedTime : m_vitesse * elapsedTime;
	

	//Selon la touche appuié et l'orientation on determine la direction que le personnage avance
	if (front)
	{
		m_posX += cos(PI / 2 + orientationPlayer) * multiplieur;
		m_posZ += sin(PI / 2 + orientationPlayer) * multiplieur;

		//deplacement verticale
		if (m_noClip)
			m_posY += sin(-m_rotY * PI / 180) * multiplieur;

	}
	if (back)
	{
		m_posX += cos(PI * 1.5 + orientationPlayer) * multiplieur;
		m_posZ += sin(PI * 1.5 + orientationPlayer) * multiplieur;

		//deplacement verticale
		if (m_noClip)
			m_posY += sin(m_rotY * PI / 180) * multiplieur;

	}
	if (right)
	{
		m_posX += cos(PI + orientationPlayer) * multiplieur;
		m_posZ += sin(PI + orientationPlayer) * multiplieur;

	}
	if (left)
	{
		m_posX += cos(0 + orientationPlayer) * multiplieur;
		m_posZ += sin(0 + orientationPlayer) * multiplieur;

	}
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
	glTranslatef(m_posX, -m_posY, m_posZ);
}

void Player::ToggleNoClip()
{
	if (m_noClip)
	{
		m_noClip = false;
		m_posY = 1.7;
	}
	else
		m_noClip = true;

}