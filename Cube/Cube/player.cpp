#include "player.h"
#include <iostream>
#include <algorithm>
#include <cmath>


Player::Player(float posX, float posY, float posZ, float rotX, float rotY) : m_pos(posX, posY, posZ), m_rotX(rotX), m_rotY(rotY),
m_vitesse(4), m_noClip(false), m_sneaked(false), m_vitesseY(0),
m_height(1.62), m_health(100), m_running(false)
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

void Player::Move(bool front, bool back, bool left, bool right, float elapsedTime, Array2d<Chunk>& chunks)
{
	float orientationPlayer = m_rotX * PI / 180;
	float multiplieur = m_vitesse * elapsedTime;
	if (m_sneaked)
		multiplieur *= 0.7;
	else if (m_noClip)
		multiplieur *= 3;
	else if (m_running)
		multiplieur *= 1.7;
	//Selon la touche appuié et l'orientation on determine la direction que le personnage avance
	if (front)
	{
		//deplacement verticale si noclip
		if (m_noClip)
		{
			m_pos.x -= cos(PI / 2 + orientationPlayer) * multiplieur * (cos(-m_rotY * PI / 180));
			m_pos.z -= sin(PI / 2 + orientationPlayer) * multiplieur * (cos(-m_rotY * PI / 180));
			m_pos.y += sin(-m_rotY * PI / 180) * multiplieur;
		}
		else
		{
			m_pos.x -= cos(PI / 2 + orientationPlayer) * multiplieur;
			if (CheckCollision(chunks))
				m_pos.x += cos(PI / 2 + orientationPlayer) * multiplieur;
			m_pos.z -= sin(PI / 2 + orientationPlayer) * multiplieur;
			if (CheckCollision(chunks))
				m_pos.z += sin(PI / 2 + orientationPlayer) * multiplieur;
		}
	}
	if (back)
	{
		//deplacement verticale si noclip
		if (m_noClip)
		{
			m_pos.x -= cos(PI * 1.5 + orientationPlayer) * multiplieur * (cos(m_rotY * PI / 180));
			m_pos.z -= sin(PI * 1.5 + orientationPlayer) * multiplieur * (cos(m_rotY * PI / 180));
			m_pos.y += sin(m_rotY * PI / 180) * multiplieur;
		}
		else
		{
			m_pos.x -= cos(PI * 1.5 + orientationPlayer) * multiplieur;
			if (CheckCollision(chunks))
				m_pos.x += cos(PI * 1.5 + orientationPlayer) * multiplieur;
			m_pos.z -= sin(PI * 1.5 + orientationPlayer) * multiplieur;
			if (CheckCollision(chunks))
				m_pos.z += sin(PI * 1.5 + orientationPlayer) * multiplieur;
		}
	}
	if (right)
	{
		m_pos.x -= cos(PI + orientationPlayer) * multiplieur;
		if (!m_noClip && CheckCollision(chunks))
			m_pos.x += cos(PI + orientationPlayer) * multiplieur;
		m_pos.z -= sin(PI + orientationPlayer) * multiplieur;
		if (!m_noClip && CheckCollision(chunks))
			m_pos.z += sin(PI + orientationPlayer) * multiplieur;
	}
	if (left)
	{
		m_pos.x -= cos(0 + orientationPlayer) * multiplieur;
		if (!m_noClip && CheckCollision(chunks) && !m_noClip)
			m_pos.x += cos(0 + orientationPlayer) * multiplieur;
		m_pos.z -= sin(0 + orientationPlayer) * multiplieur;
		if (!m_noClip && CheckCollision(chunks) && !m_noClip)
			m_pos.z += sin(0 + orientationPlayer) * multiplieur;
	}
	//Gravité
	if (!m_noClip)
	{
		//Chute
		m_pos.y -= m_vitesseY;
		if (CheckCollision(chunks))
		{
			m_air = false;
			m_pos.y += m_vitesseY;
			m_vitesseY = 0;
		}
		//Acceleration
		m_vitesseY += 0.5 * elapsedTime;
	}
}

bool Player::CheckCollision(Array2d<Chunk>& chunks)
{
	for (int i = 0; i < WORLD_SIZE; i++)
	{
		for (int j = 0; j < WORLD_SIZE; j++)	//Parcours les chunks
		{
			Vector3<float> Chunk = chunks.Get(i, j).GetPosition();

			//Si le player est dans le chunk
			if (m_pos.x >= Chunk.x
				&& m_pos.x < Chunk.x + CHUNK_SIZE_X
				&& m_pos.y >= Chunk.y
				&& m_pos.y < Chunk.y + CHUNK_SIZE_Y
				&& m_pos.z >= Chunk.z
				&& m_pos.z < Chunk.z + CHUNK_SIZE_Z)
			{
				for (int x = 0; x < CHUNK_SIZE_X; ++x)
				{
					for (int z = 0; z < CHUNK_SIZE_Z; ++z)
					{
						for (int y = 0; y < CHUNK_SIZE_Y; ++y)	//parcours les blocks du chunk
						{
							if (chunks.Get(i, j).GetBlock(x, y, z) != BTYPE_AIR)
							{
								Vector3<float> block = chunks.Get(i, j).GetBlockPos(x, y, z);

								//Si le player est dans le block
								if (//Pied
									(m_pos.x >= block.x
									&& m_pos.x < block.x + 1
									&& m_pos.y >= block.y
									&& m_pos.y < block.y + 1
									&& m_pos.z >= block.z
									&& m_pos.z < block.z + 1) ||
									//Millieu du corps
									(m_pos.x >= block.x
									&& m_pos.x < block.x + 1
									&& m_pos.y + m_height >= block.y
									&& m_pos.y + m_height < block.y + 1
									&& m_pos.z >= block.z
									&& m_pos.z < block.z + 1) ||
									//Tete
									(m_pos.x >= block.x
									&& m_pos.x < block.x + 1
									&& m_pos.y + m_height / 2 >= block.y
									&& m_pos.y + m_height / 2 < block.y + 1
									&& m_pos.z >= block.z
									&& m_pos.z < block.z + 1))
								{
									return true; //colision
								}
							}
						}
					}
				}
				return false;
			}
		}
	}
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
	glTranslatef(-m_pos.x, -(m_pos.y + m_height), -m_pos.z);

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