#ifndef PLAYERACTOR_H__
#define PLAYERACTOR_H__

#include "character.h"
#include "vector3.h"
#include "model3d.h"


class PlayerActor : public Character
{
public:
	PlayerActor();
	~PlayerActor();
	void Draw(Model3d &model) const;
	void SetPos(const Vector3<float> &pos);
	void SetRot(float hRot);
};
#endif // PLAYERACTOR_H__


