#include "playerActor.h"


PlayerActor::PlayerActor() : Character()
{
}


PlayerActor::~PlayerActor()
{
}

void PlayerActor::SetPos(const Vector3<float> &pos)
{
	m_pos = pos;
}

void PlayerActor::SetRot(float hRot)
{
	m_HorizontalRot = hRot;
}

void PlayerActor::Draw(Model3d &model) const
{
	if (m_isAlive)
		model.Render(m_pos.x, m_pos.y, m_pos.z, -m_HorizontalRot + 180, 0, 1.f, 0.5f, 1.f);
}