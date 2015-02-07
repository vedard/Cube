#include "bullet.h"


Bullet::Bullet() : m_LastPos(0, 0, 0)
{
	m_damage = 50;
	m_isActive = false;
	m_vitesse.x = 20;
	m_vitesse.y = 20;
	m_vitesse.z = 20;
	m_distance = 0;


}

Bullet::~Bullet()
{

}

void Bullet::Update()
{
	if (m_isActive)
	{
		m_LastPos = m_pos;

		m_pos.y += directionVector.y;
		m_pos.x += directionVector.x;
		m_pos.z += directionVector.z;

		//Si la balle est trop loin, elle est detruite
		m_distance += directionVector.Length();
		if (m_distance > CHUNK_SIZE_X * 10)
			m_isActive = false;
	}

}

bool Bullet::CheckCollision(Character &character)
{
	if (m_isActive && character.GetisAlive())
	{
		int nbrIteration = 10;
		if (character.GetDimension().x <= character.GetDimension().y && character.GetDimension().x <= character.GetDimension().z)
			nbrIteration = ceil(m_vitesse.x / character.GetDimension().x);
		if (character.GetDimension().y <= character.GetDimension().x && character.GetDimension().y <= character.GetDimension().z)
			nbrIteration = ceil(m_vitesse.y / character.GetDimension().y);
		if (character.GetDimension().z <= character.GetDimension().y && character.GetDimension().z <= character.GetDimension().x)
			nbrIteration = ceil(m_vitesse.z / character.GetDimension().z);

		for (int i = 0; i < nbrIteration; i++)
		{
			if (m_LastPos.x + directionVector.x / nbrIteration * i >= character.GetPosition().x - character.GetDimension().x / 2
				&& m_LastPos.x + directionVector.x / nbrIteration * i < character.GetPosition().x + character.GetDimension().x / 2
				&& m_LastPos.y + directionVector.y / nbrIteration * i >= character.GetPosition().y
				&& m_LastPos.y + directionVector.y / nbrIteration * i < character.GetPosition().y + character.GetDimension().y
				&& m_LastPos.z + directionVector.z / nbrIteration * i >= character.GetPosition().z - character.GetDimension().z / 2
				&& m_LastPos.z + directionVector.z / nbrIteration * i < character.GetPosition().z + character.GetDimension().z / 2)
			{

				//f(x) = -1 * 1.04 ^ (x + 10) + 50
				character.GetDamage(-1 * pow(1.04, m_distance + 10) + 50);
				m_isActive = false;
				return true;
			}
		}
	}
	return false;
}
bool Bullet::CheckCollision(World &world)
{
	if (m_isActive)
	{
		BlockType bt1 = world.BlockAt(m_pos.x, m_pos.y, m_pos.z);

		//Si un des block n'est pas BTYPE_AIR OU BTYPE_WATER -> il y a collision
		if (bt1 != BTYPE_AIR && bt1 != BTYPE_WATER)
		{
			/*Vector3<float> chunkPos(floor(m_pos.x / CHUNK_SIZE_X), 0, floor(m_pos.z / CHUNK_SIZE_Z));
			Chunk * chunk = world.ChunkAt(chunkPos.x, chunkPos.z);

			if (chunk)
			chunk->RemoveBloc(m_pos.x - (chunkPos.x * CHUNK_SIZE_X), m_pos.y, m_pos.z - (chunkPos.z * CHUNK_SIZE_X));*/
			m_isActive = false;
			return true;

		}
	}
	return false;

}

void Bullet::Draw() const
{
	if (m_isActive)
	{

		float width = 0.03;

		glPushMatrix();
		glTranslatef(m_pos.x, m_pos.y, m_pos.z);
		glColor3f(0.f, 0.f, 0.f);

		glBegin(GL_QUADS);
		glVertex3f(-width / 2, 0, -width / 2);
		glVertex3f(-width / 2, width, -width / 2);
		glVertex3f(width / 2, width, -width / 2);
		glVertex3f(width / 2, 0, -width / 2);

		glVertex3f(width / 2, width, -width / 2);
		glVertex3f(width / 2, width, width / 2);
		glVertex3f(width / 2, 0, width / 2);
		glVertex3f(width / 2, 0, -width / 2);

		glVertex3f(-width / 2, 0, width / 2);
		glVertex3f(width / 2, 0, width / 2);
		glVertex3f(width / 2, width, width / 2);
		glVertex3f(-width / 2, width, width / 2);

		glVertex3f(-width / 2, width, width / 2);
		glVertex3f(-width / 2, width, -width / 2);
		glVertex3f(-width / 2, 0, -width / 2);
		glVertex3f(-width / 2, 0, width / 2);

		glVertex3f(-width / 2, width, width / 2);
		glVertex3f(width / 2, width, width / 2);
		glVertex3f(width / 2, width, -width / 2);
		glVertex3f(-width / 2, width, -width / 2);

		glVertex3f(-width / 2, 0, width / 2);
		glVertex3f(-width / 2, 0, -width / 2);
		glVertex3f(width / 2, 0, -width / 2);
		glVertex3f(width / 2, 0, width / 2);
		glEnd();

		glPopMatrix();

	}
}
void Bullet::Init(float x, float y, float z, float rotationVertical, float rotationHorizontal)
{
	m_pos = Vector3<float>(x, y, z);
	directionVector = Vector3<float>(
		cosf(PI / 2 * 3 + rotationHorizontal * PI / 180) * (cosf(-rotationVertical * PI / 180)),
		sinf(-rotationVertical * PI / 180),
		sinf(PI / 2 * 3 + rotationHorizontal * PI / 180) * (cosf(-rotationVertical * PI / 180)));

	directionVector.Normalize();
	directionVector.y *= m_vitesse.y;
	directionVector.x *= m_vitesse.x;
	directionVector.z *= m_vitesse.z;
	m_distance = 0;
	m_isActive = true;
}

const Vector3<float>& Bullet::GetPosition() const
{
	return m_pos;
}

float Bullet::GetDamage() const
{
	return m_damage;

}

bool Bullet::GetIsActive() const
{
	return m_isActive;
}

void Bullet::SetInactive()
{
	m_isActive = false;
}
