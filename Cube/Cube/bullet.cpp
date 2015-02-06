#include "bullet.h"


Bullet::Bullet()
{
	m_damage = 50;
	m_isActive = false;
	m_vitesse.x = 2;
	m_vitesse.y = 2;
	m_vitesse.z = 2;
	m_distance = 0;

}

Bullet::~Bullet()
{

}

void Bullet::Update()
{
	if (m_isActive)
	{
		Vector3<float> directionVector(
			cosf(PI / 2 * 3 + m_HorizontalRot * PI / 180) * (cosf(-m_VerticalRot * PI / 180)),
			sinf(-m_VerticalRot * PI / 180),
			sinf(PI / 2 * 3 + m_HorizontalRot * PI / 180) * (cosf(-m_VerticalRot * PI / 180)));

		directionVector.Normalize();

		m_pos.y += directionVector.y * m_vitesse.y;
		m_pos.x += directionVector.x * m_vitesse.x;
		m_pos.z += directionVector.z * m_vitesse.z;

		//Si la balle est trop loin, elle est detruite
		m_distance += directionVector.Length() * m_vitesse.y;
		if (m_distance > CHUNK_SIZE_X * 10)
			m_isActive = false;
	}

}

bool Bullet::CheckCollision(Character &character)
{
	if (m_isActive && character.GetisAlive())
	{
		if (m_pos.x >= character.GetPosition().x - character.GetDimension().x / 2
			&& m_pos.x < character.GetPosition().x + character.GetDimension().x / 2
			&& m_pos.y >= character.GetPosition().y - character.GetDimension().y
			&& m_pos.y < character.GetPosition().y + character.GetDimension().y
			&& m_pos.z >= character.GetPosition().z - character.GetDimension().z / 2
			&& m_pos.z < character.GetPosition().z + character.GetDimension().z / 2)
		{
			//f(x) = -1 * 1.04 ^ (x + 10) + 50
			character.GetDamage(-1 * pow(1.04, m_distance + 10) + 50);
			m_isActive = false;
			return true;
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
	m_HorizontalRot = rotationHorizontal;
	m_VerticalRot = rotationVertical;
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
