#ifndef BLOODMOON_H__
#define BLOODMOON_H__
#include "define.h"
#include "sound.h"

class BloodMoon
{
public:
	BloodMoon();
	~BloodMoon();

	void Activate();				// Permet au start de commencerà
	void Deactivate();
	void Start();					// mets start a true
	void Stop();
	void AddElapsedUnit();
	bool GetCompletionState();
	bool GetActiveState() const;
	bool GetStartedState() const;
	float GetDuration() const;

	float monsterMultiplier;

private:
	void Reset();

	bool m_isActive;
	bool m_isStarted;
	float m_duration;
	float m_currTime;
};
#endif