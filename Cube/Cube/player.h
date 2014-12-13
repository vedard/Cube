#ifndef PLAYER_H__
#define PLAYER_H__
#include "define.h"
#include "vector3.h"
#include "array2d.h"
#include "chunk.h"
#include "world.h"

class Player
{

public:

	Player();
	~Player();
	void Spawn(World &world);
	void TurnLeftRight(float value);
	void TurnTopBottom(float value);
	void Move(bool front, bool back, bool left, bool right, World &world);
	bool CheckCollision(World &world) const;

	void ApplyRotation() const;
	void ApplyTranslation() const;
	void ToggleNoClip();
	void SetSneak(bool sneak);
	void SetRunning(bool running);
	void Jump();
	void Hurt(int damage);
	int GetHP() const;
	BlockType GetBlock() const;

	// negative < 0 < positif
	void SetBlock(int direction);
	Vector3<float> Position() const;
	Vector3<float> GetDimension() const;

private:
	
	Vector3<float> m_pos;
	Vector3<float> m_dimension;

	float m_rotX,  //orientation
		m_rotY, 
		m_vitesse; //vitesse de deplacement

	float m_vitesseY;   //Vittesse verticale (gravité et saut)
	bool m_noClip;		//Si on est en noclip mode (sans collision et vol)
	bool m_sneaked;		//Si on est penché
	bool m_air;			//Si on est dans les airs (ex: pendans un saut ou une chute)
	int m_health;		//Vie du personnage
	bool m_running;		//Si le joueur cour
	BlockType m_block;   //Prochain block que le joueur peut placer
};

#endif