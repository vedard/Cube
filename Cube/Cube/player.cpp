#include "player.h"
#include <iostream>
#include <algorithm>
#include <cmath>


Player::Player(float posX, float posY, float posZ, float rotX, float rotY) : m_pos(posX, posY, posZ), m_rotX(rotX), m_rotY(rotY),
m_vitesse(4), m_noClip(false), m_sneaked(false), m_vitesseY(0),
m_height(1.62), m_health(100), m_running(false), m_width(0.2)
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
	//Orientation du player en rad
	float orientationPlayer = m_rotX * PI / 180;
	//Multiplicateur de vitesse
	float multiplieur = m_vitesse * elapsedTime;

	//Change la vittesse selon l'etat du player
	if (m_sneaked)
		multiplieur *= 0.7;
	else if (m_noClip)
		multiplieur *= 3;
	else if (m_running)
		multiplieur *= 1.7;

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
		if (CheckCollision(chunks)) 
			m_pos.x -= deplacementVector.x * multiplieur;

		//Deplacement en Z
		m_pos.z += deplacementVector.z * multiplieur;
		//Si collision, on annule
		if (CheckCollision(chunks))
			m_pos.z -= deplacementVector.z * multiplieur;
	}

	//Gravité
	if (!m_noClip)
	{
		//Chute
		m_pos.y -= m_vitesseY;

		//Si collision
		if (CheckCollision(chunks))
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

								if ((m_pos.x - (m_width / 2) >= block.x + 1)	// trop à droite
									|| (m_pos.x + (m_width / 2) <= block.x)		// trop à gauche
									|| (m_pos.y >= block.y + 1)					// trop en bas
									|| (m_pos.y + m_height <= block.y)			// trop en haut	
									|| (m_pos.z - (m_width / 2) >= block.z + 1)	// trop derrière
									|| (m_pos.z + (m_width / 2) <= block.z))	// trop devant
									continue;

								else
									return true; //Sinon collision

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