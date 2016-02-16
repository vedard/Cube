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

	bool m_isSurvivePlayed;
	bool m_isHorrorPlayed;

private:
	bool m_isActive;
	bool m_isStarted;

	float m_duration;
	float m_currTime;
};
#endif