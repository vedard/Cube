#include "player.h"
#include <iostream>
#include <algorithm>
#include <cmath>


Player::Player(float posX, float posY, float posZ, float rotX, float rotY) : m_pos(posX, posY, posZ), m_rotX(rotX), m_rotY(rotY), 
																			 m_vitesse(4), m_noClip(false), m_sneaked(false), m_vitesseY(0), 
																			 m_height(1.62), m_health(100)
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
	float multiplieur = m_vitesse * elapsedTime;

	if (m_sneaked)
		multiplieur *= 0.7;
	else if (m_noClip)
		multiplieur *= 3;
	else if (shift)
		multiplieur *= 1.7;

	//Selon la touche appuié et l'orientation on determine la direction que le personnage avance
	if (front)
	{

		//deplacement verticale si noclip
		if (m_noClip)
		{
			m_pos.x += cos(PI / 2 + orientationPlayer) * multiplieur * (cos(-m_rotY * PI / 180));
			m_pos.z += sin(PI / 2 + orientationPlayer) * multiplieur * (cos(-m_rotY * PI / 180));
			m_pos.y += sin(-m_rotY * PI / 180) * multiplieur;
		}

		else
		{
			m_pos.x += cos(PI / 2 + orientationPlayer) * multiplieur;
			m_pos.z += sin(PI / 2 + orientationPlayer) * multiplieur;
		}
	}
	if (back)
	{
		//deplacement verticale si noclip
		if (m_noClip)
		{
			m_pos.x += cos(PI * 1.5 + orientationPlayer) * multiplieur * (cos(m_rotY * PI / 180));
			m_pos.z += sin(PI * 1.5 + orientationPlayer) * multiplieur * (cos(m_rotY * PI / 180));
			m_pos.y += sin(m_rotY * PI / 180) * multiplieur;
		}

		else
		{
			m_pos.x += cos(PI * 1.5 + orientationPlayer) * multiplieur;
			m_pos.z += sin(PI * 1.5 + orientationPlayer) * multiplieur;
		}
	}
	if (right)
	{
		m_pos.x += cos(PI + orientationPlayer) * multiplieur;
		m_pos.z += sin(PI + orientationPlayer) * multiplieur;

	}
	if (left)
	{
		m_pos.x += cos(0 + orientationPlayer) * multiplieur;
		m_pos.z += sin(0 + orientationPlayer) * multiplieur;

	}

	//Gravité
	if (!m_noClip)
	{
		//Chute
		m_pos.y -= m_vitesseY;
		//Acceleration
		m_vitesseY += 0.5 * elapsedTime;

		//Si on touche le sol
		if (m_pos.y <= 0)
		{
			m_pos.y = 0;
			m_vitesseY = 0;
			m_air = false;
		}
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
	glTranslatef(m_pos.x, -(m_pos.y + m_height), m_pos.z);

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

Vector3<float> Player::Position() const
{
	return m_pos;
}

void Player::Jump()
{
	if (!m_air)
	{
		m_vitesseY = -0.20;
		m_air = true;
	}
}

int Player::GetHP()
{
	return m_health;
}