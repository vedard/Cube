#ifndef PLAYER_H__
#define PLAYER_H__
#include "define.h"
#include "vector3.h"

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
	void SetSneak(bool sneak);
	Vector3<float> Position() const;

private:
	
	Vector3<float> m_pos;
	float m_rotX,  //orientation
		m_rotY, 
		m_vitesse; //vitesse de deplacement

	bool m_noClip;
	bool m_sneaked;
};

#endif