#ifndef ANIMAL_H__
#define ANIMAL_H__

#include "character.h"
#include "model3d.h"
#include "sound.h"

class Animal : public Character
{
public:
	Animal(ANIMAL_TYPE typ);
	~Animal();
	
    void Move(World &world);
	void Draw(Model3d &model) const;
	virtual void GetDamage(float damage);
	virtual void SetTarget(Character* target);
	ANIMAL_TYPE GetType();

	 
private:
	sf::Clock m_ClockTarget;
	int m_timeNextTarget;
	ANIMAL_TYPE type;
	sf::Clock m_ClockAnimationDmg;

};

#endif