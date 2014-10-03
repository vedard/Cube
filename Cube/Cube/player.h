#ifndef PLAYER_H__
#define PLAYER_H__
#include "define.h"

class Player
{

public:

	Player(float posX = 0, float posY = 0, float posZ = 0, float rotX = 0, float rotY = 0);
	~Player();
	void TurnLeftRight(float value);
	void TurnTopBottom(float value);
	void Move(bool front, bool back, bool left, bool right,bool shift, float elapsedTime);
	void ApplyRotation() const;
	void ApplyTranslation() const;
	void ToggleNoClip();

private:
	float m_posX, //position
		m_posY, 
		m_posZ, 
		m_rotX,  //orientation
		m_rotY, 
		m_vitesse; //vitesse de deplacement

	bool m_noClip;
};

#endif