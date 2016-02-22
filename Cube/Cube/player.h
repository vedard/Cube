#ifndef PLAYER_H__
#define PLAYER_H__
#include "array2d.h"
#include "character.h"
#include "gun.h"
#include "inventory.h"
#include "define.h"
#include "experience.h"
#include <memory>
#include <cmath>


class Player : public Character
{

public:

	Player();
	~Player();

	void TurnLeftRight(float value);
	void TurnTopBottom(float value);
	void Move(bool front, bool back, bool left, bool right, World &world);
	void DeathCheck();
	void ApplyRotation() const;
	float ApplyTranslation(); // return le shake
	void ToggleNoClip();
	void Jump();
	void Tick();
	bool Shoot(World &world);

	bool Underwater() const;
	bool footUnderwater() const;
	BlockType blockUnderPlayer() const;
	bool UnderLava() const;

	BlockType GetBlock() const;
	int GetWeapon() const;
	Gun* GetGuns() const;

	// negative < 0 < positif
	void SetBlock(int direction);
	void SetBlockDirect(BlockType blockType);
	void SetWeapon(int mode);
	void SetSneak(bool sneak);
	void SetRunning(bool running);

	bool GetIsSneaked();
	bool GetDamage(float damage, bool ignoreArmor, bool godMode, Sound::ListeSons son = Sound::HURT, bool playonce = false) override;

	void AddToInventory(BlockType type);
	bool RemoveFromInventory(BlockType type);
	Inventory* GetInventory();
	
	Experience* GetXp() { return &m_xp; }

private:
	void CheckUnderwater(World &world);
	void CheckUnderLava(World &world);
	void CheckBlockUnder(World &world);
	void ResetDeath();
private:
	bool m_wasInAir; // S'il etait dans les airs.
	bool m_godMode;
	time_t m_TickBegin;     //timer de tick
	time_t m_TickEnd;     //timer de tick
	int  m_BreathCount; //Compte les ticks pour la breath
	bool m_noClip;			// Si on est en noclip mode (sans collision et vol)
	bool m_sneaked;			// Si on est penché
	bool m_running;			// Si le joueur cour
	bool m_headUnderwater;  // Si le joueur est sous l'eau
	bool m_headWasUnderwater = false; // Si le joueur vient de sortir de l'eau.
	bool m_footUnderwater;  // Si le joueur est sous l'eau
	bool m_footWasUnderwater; // si le joueur vient de sortir de l'eau
	bool m_headUnderLava; // Si le joueur est sous la lave
	bool m_kneeUnderwater;  // Si le joueur est sous l'eau
	bool m_kneeUnderLava;  // Si le joueur est sous la lave
	bool m_footUnderLava;  // Si le joueur est sous la lave
	BlockType m_block;// Prochain block que le joueur peut placer
	BlockType m_blockUnder; // Bloc en dessous du joueur
	Gun * Guns;
	float InvulnerabilityTimer;
	
	int m_weapon;			// current weapon

	float m_HeadShake;		// Shake la camera a chaque pas

	std::unique_ptr<Inventory> m_inventory;	//Inventaire du joueur

	Experience m_xp;

public:
	int isHurt;
	float hasHit;
};

#endif
