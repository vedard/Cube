#ifndef PLAYER_H__
#define PLAYER_H__
#include "define.h"
#include "vector3.h"
#include "array2d.h"
#include "chunk.h"

class Player
{

public:

	Player(float posX = 0, float posY = 0, float posZ = 0, float rotX = 0, float rotY = 0);
	~Player();
	void TurnLeftRight(float value);
	void TurnTopBottom(float value);
	void Move(bool front, bool back, bool left, bool right, bool shift, float elapsedTime, Array2d<Chunk>& chunks);
	bool CheckCollision(Array2d<Chunk>& chunks);

	void ApplyRotation() const;
	void ApplyTranslation() const;
	void ToggleNoClip();
	void SetSneak(bool sneak);
	void Jump();
	int GetHP();
	Vector3<float> Position() const;

private:
	
	Vector3<float> m_pos;
	float m_rotX,  //orientation
		m_rotY, 
		m_vitesse; //vitesse de deplacement
	float m_vitesseY;   //Vittesse verticale (gravité et saut)
	float m_height;     //Position des yeux relativement aux pied

	bool m_noClip;		//Si on est en noclip mode (sans collision et vol)
	bool m_sneaked;		//Si on est penché
	bool m_air;			//Si on est dans les airs (ex: pendans un saut ou une chute)

	int m_health;		//Vie du personnage
};

#endif