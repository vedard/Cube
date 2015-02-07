#ifndef ANIMAL_H__
#define ANIMAL_H__

#include "character.h"
#include "model3d.h"

class Animal : public Character
{
public:
	Animal();
	~Animal();
	
	void Move(World &world);
	void Draw(Model3d &model) const;
	void GetDamage(float damage);

	 
private:
	sf::Clock m_ClockTarget;
	int m_timeNextTarget;

	sf::Clock m_ClockAnimationDmg;

};

#endif