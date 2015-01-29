#ifndef PLAYER_H__
#define PLAYER_H__
#include "array2d.h"
#include "character.h"
#include "bullet.h"


class Player : public Character
{

public:

	Player();
	~Player();

	void TurnLeftRight(float value);
	void TurnTopBottom(float value);
	void Move(bool front, bool back, bool left, bool right, World &world);
	void ApplyRotation() const;
	void ApplyTranslation() const;
	void ToggleNoClip();
	void SetSneak(bool sneak);
	void SetRunning(bool running);
	void Jump();
	void Shoot();

	bool Underwater() const;

	BlockType GetBlock() const;
	int GetWeapon() const;

	// negative < 0 < positif
	void SetBlock(int direction);
	void SetWeapon(int mode);




private:
	void CheckUnderwater(World &world);

private:

	bool m_noClip;			// Si on est en noclip mode (sans collision et vol)
	bool m_sneaked;			// Si on est penché
	bool m_running;			// Si le joueur cour
	bool m_headUnderwater;  // Si le joueur est sous l'eau
	bool m_footUnderwater;  // Si le joueur est sous l'eau
	BlockType m_block;		// Prochain block que le joueur peut placer
	
	int m_weapon;

public:
	Bullet m_bullet;
};

#endif