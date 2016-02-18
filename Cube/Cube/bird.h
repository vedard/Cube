#ifndef BIRD_H__
#define BIRD_H__

#include "animal.h"

class Bird : public Animal
{
public:
	Bird();
	~Bird();
	void Move(World &world) override;
private:
	bool m_falling;
};
#endif // !COW_H__

