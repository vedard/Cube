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
	bool Underwater() const;
	BlockType GetBlock() const;

	// negative < 0 < positif
	void SetBlock(int direction);
	Vector3<float> Position() const;
	Vector3<float> GetDimension() const;

private:
	void CheckUnderwater(World &world);

private:
	
	Vector3<float> m_pos;
	Vector3<float> m_dimension;
	Vector3<float> m_vitesse;

	float m_rotX,  //orientation
		m_rotY;
	
	int m_health;		//Vie du personnage

	bool m_noClip;		//Si on est en noclip mode (sans collision et vol)
	bool m_sneaked;		//Si on est pench�
	bool m_air;			//Si on est dans les airs (ex: pendans un saut ou une chute)
	bool m_running;		//Si le joueur cour
	bool m_headUnderwater;  // Si le joueur est sous l'eau
	bool m_footUnderwater;  // Si le joueur est sous l'eau
	BlockType m_block;   //Prochain block que le joueur peut placer
};

#endif