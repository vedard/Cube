#ifndef PLAYER_H__
#define PLAYER_H__
#include "array2d.h"
#include "character.h"
#include "gun.h"


class Player : public Character
{

public:

	Player();
	~Player();

	void TurnLeftRight(float value);
	void TurnTopBottom(float value);
	void Move(bool front, bool back, bool left, bool right, World &world);
	void ApplyRotation() const;
	void ApplyTranslation();
	void ToggleNoClip();
	void Jump();
	void Tick();


	bool Underwater() const;
	bool UnderLava() const;

	BlockType GetBlock() const;
	int GetWeapon() const;

	// negative < 0 < positif
	void SetBlock(int direction);
	void SetWeapon(int mode);
	void SetSneak(bool sneak);
	void SetRunning(bool running);



private:
	void CheckUnderwater(World &world);
	void CheckUnderLava(World &world);

private:
	time_t m_TickBegin;     //timer de tick
	time_t m_TickEnd;     //timer de tick
	int  m_BreathCount; //Compte les ticks pour la breath
	bool m_noClip;			// Si on est en noclip mode (sans collision et vol)
	bool m_sneaked;			// Si on est penché
	bool m_running;			// Si le joueur cour
	bool m_headUnderwater;  // Si le joueur est sous l'eau
	bool m_footUnderwater;  // Si le joueur est sous l'eau
	bool m_headUnderLava;  // Si le joueur est sous la lave
	bool m_kneeUnderwater;  // Si le joueur est sous l'eau
	bool m_kneeUnderLava;  // Si le joueur est sous la lave
	bool m_footUnderLava;  // Si le joueur est sous la lave
	BlockType m_block;		// Prochain block que le joueur peut placer
	
	int m_weapon;			// current weapon

	float m_HeadShake;		// Shake la camera a chaque pas
};

#endif